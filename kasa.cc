#include <algorithm>
#include <climits>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using time_point = std::pair<int, int>;
using stops = std::unordered_map<std::string, time_point>;
// price of the ticket is multiplied by 100
// (name, price, duration)
using single_ticket = std::tuple<std::string, long long, long long>;
// each element contains name of the stop and number of the course
using travel = std::vector<std::pair<std::string, long long>>;

static const std::regex add_course_regex(
    "(\\d+)((?: (?:(?:\\d|1\\d|2[0-3]):[0-5]\\d) (?:[a-zA-Z_^]+))+)");
static const std::regex add_ticket_regex("([a-zA-Z ]+) (\\d+)\\.(\\d{2}) ((?!0)\\d+)");
static const std::regex travel_query_regex("\\? ([a-zA-Z_^]+(?: \\d+ [a-zA-Z_^]+)+)");

static const time_point opening_time = std::make_pair(5, 55);
static const time_point closing_time = std::make_pair(21, 21);

bool equal_time_points(time_point a, time_point b) {
  if (a.first == b.first && a.second == b.second) return true;
  return false;
}

long long count_time_distance(time_point a, time_point b) {
  long long time_distance = abs(b.second - a.second) + 1;
  time_distance += (abs(b.first - a.first) * 60);
  return time_distance;
}

// travel_time - czas, na który mamy kupić bilety podany w minutach
std::vector<std::string> buy_tickets(std::vector<single_ticket> tickets, long long travel_time) {
  long long smallest_cost = LLONG_MAX;
  std::vector<std::string> best_tickets;

  for (size_t i = 0; i < tickets.size(); i++) {
    long long summed_time = std::get<2>(tickets[i]);
    long double summed_cost = std::get<1>(tickets[i]);
    if (travel_time >= summed_time && summed_cost < smallest_cost) {
      smallest_cost = summed_cost;
      best_tickets.clear();
      best_tickets.push_back(std::get<0>(tickets[i]));
      continue;
    }
    for (size_t j = i; j < tickets.size(); j++) {
      summed_cost = std::get<1>(tickets[i]) + std::get<1>(tickets[j]);
      summed_time = std::get<2>(tickets[i]) + std::get<2>(tickets[j]);
      if (travel_time >= summed_time && summed_cost < smallest_cost) {
        smallest_cost = summed_cost;
        best_tickets.clear();
        best_tickets.push_back(std::get<0>(tickets[i]));
        best_tickets.push_back(std::get<0>(tickets[j]));
        continue;
      }
      for (size_t k = j; k < tickets.size(); k++) {
        summed_cost = std::get<1>(tickets[i]) + std::get<1>(tickets[j]) + std::get<1>(tickets[k]);
        summed_time = std::get<2>(tickets[i]) + std::get<2>(tickets[j]) + std::get<2>(tickets[k]);
        if (travel_time >= summed_time && summed_cost < smallest_cost) {
          best_tickets.clear();
          best_tickets.push_back(std::get<0>(tickets[i]));
          best_tickets.push_back(std::get<0>(tickets[j]));
          best_tickets.push_back(std::get<0>(tickets[k]));
          smallest_cost = summed_cost;
          break;
        }
      }
    }
  }
  return best_tickets;
}

// current_travel zawiera opis niepustej podróży
// funkcja time_of_connection zwraca nieujemną liczbę reprezentującą
// sumaryczny czas podróży
// lub liczbę ujemną równą current_travel.size(), gdy dane wejściowe są niepoprawne
// inna liczba ujemna reprezentuje indeks przystanku, na którym trzeba czekać
int time_of_connection(std::unordered_map<long long, stops> courses, travel current_travel) {
  time_point current_time, start_point;

  for (size_t i = 0; i < current_travel.size() - 1; i++) {
    int course_number = current_travel[i].second;

    if (courses.find(course_number) == courses.end()) {
      // nie istnieje kurs o zadanym numerze
      return current_travel.size() * (-1);
    }
    stops current_course = courses[course_number];

    int current_stop_name = current_travel[i].first;
    int next_stop_name = current_travel[i + 1].first;

    if (current_course.find(current_stop_name) == courses.end() ||
        current_course.find(next_stop_name) == courses.end()) {
      // w podanym kursie nie ma takiego przystanku
      return current_travel.size() * (-1);
    }

    time_point departure_from_stop = current_course[current_stop_name];
    time_point arrival_for_the_stop = current_course[next_stop_name];

    if (i == 0) {
      current_time = departure_from_stop;
      start_point = departure_from_stop;
    } else {
      if (equal_time_points(current_time, departure_from_stop)) {
        // trzeba czekać na tym przystanku
        return -i;
      } else {
        current_time = arrival_for_the_stop;
      }
    }
  }

  return count_time_distance(current_time, start_point);
}

void string_split(const std::string& str, std::vector<std::string>& v, char d = ' ') {
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, d)) {
    if (token.length() > 0) v.push_back(token);
  }
}

void print_error(size_t line_number, std::string line) {
  std::cerr << "Error in line " << line_number << ": " << line << "\n";
}

std::pair<long long, stops> parse_course(std::smatch sm) {
  long long number =
      std::stoll(sm[1].str());  // TODO: handle exceptions to catch numbers that are too big
  stops course;

  std::vector<std::string> v;
  string_split(sm[2].str(), v);
  time_point previous = opening_time;
  for (int i = 0; i < (int)v.size() - 1; i += 2) {
    std::vector<std::string> time;
    string_split(v[i], time, ':');
    time_point tp = std::make_pair(std::stoi(time[0]), std::stoi(time[1]));
    if (tp <= previous) {
      // TODO: handle error
    }
    previous = tp;
    std::string stop = v[i + 1];
    if (course.count(stop)) {
      // TODO: handle error
    }
    course[stop] = tp;
  }

  return std::make_pair(number, course);
}

single_ticket parse_ticket(std::smatch sm) {
  std::string name = sm[1].str();
  // Część całkowita i ułamkowa w osobnych grupach, by łatwo było je złączyć
  long long price = std::stoll(sm[2].str() + sm[3].str());
  long long duration =
      std::stoll(sm[4].str());  // TODO: handle exceptions to catch numbers that are too big
  return std::make_tuple(name, price, duration);
}

travel parse_travel_query(std::smatch sm) {
  std::string travel_str = sm[1].str();
  std::vector<std::string> args;
  string_split(travel_str, args);
  travel result;

  for (int i = 0; i < (int)args.size(); i += 2) {
    long long nr;
    if (i == (int)args.size() - 1) {
      nr = std::stoll(args[i + 1]);
      // TODO: check if number exists?
    } else {
      nr = -1;
    }
    std::string name = args[i];
    // TOFO check if stop exists?
    result.push_back(std::make_pair(name, nr));
  }

  return result;
}

int main(int argc, char* argv[]) {
  // courses: numer linii -> rozkład
  std::unordered_map<long long, stops> courses;
  // tickets: lista biletów
  std::vector<single_ticket> tickets;

  std::string line;
  size_t line_number = 0;

  while (std::getline(std::cin, line)) {
    line_number++;
    if (line.length() == 0) continue;

    std::smatch sm;
    if (std::regex_match(line, sm, add_course_regex)) {
      std::pair<long long, stops> course = parse_course(sm);
      if (courses.count(course.first)) {
        // TODO: handle error
      }
      courses[course.first] = course.second;
    } else if (std::regex_match(line, sm, add_ticket_regex)) {
      single_ticket ticket = parse_ticket(sm);
      for (auto t : tickets) {
        if (std::get<0>(t) == std::get<0>(ticket)) {
          // TODO: handle error
        }
      }
      tickets.push_back(ticket);
    } else if (std::regex_match(line, sm, travel_query_regex)) {
      travel query = parse_travel_query(sm);
    } else {
      print_error(line_number, line);
    }
  }
}
