#include <algorithm>
#include <climits>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

// para reprezentujaca odpowiednio godziny i minuty
using time_point = std::pair<int, int>;
using stops = std::unordered_map<std::string, time_point>;
// cena biletu jest przemnożona przez 100
// (nazwa, cena, czas ważności)
using single_ticket = std::tuple<std::string, long long, long long>;
// każdy element zawiera nazwę przystanku i numer linii
using travel = std::vector<std::pair<std::string, long long>>;

static const std::regex add_course_regex(
    "(\\d+)((?: (?:(?:\\d|1\\d|2[0-3]):[0-5]\\d) (?:[a-zA-Z_^]+))+)");
static const std::regex add_ticket_regex("([a-zA-Z ]+) (\\d+)\\.(\\d{2}) ((?!0)\\d+)");
static const std::regex travel_query_regex("\\? ([a-zA-Z_^]+(?: \\d+ [a-zA-Z_^]+)+)");

static const time_point opening_time = std::make_pair(5, 55);
static const time_point closing_time = std::make_pair(21, 21);

long long count_time_distance(time_point a, time_point b) {
  long long time_distance = abs(b.second - a.second);
  if (b.second != a.second) time_distance += 1;
  time_distance += (abs(b.first - a.first) * 61);
  return time_distance;
}

// travel_time - czas, na który mamy kupić bilety podany w minutach
std::vector<std::string> buy_tickets(const std::vector<single_ticket>& tickets,
                                     long long travel_time) {
  long long smallest_cost = LLONG_MAX;
  std::vector<std::string> best_tickets;

  for (size_t i = 0; i < tickets.size(); i++) {
    long long summed_time = std::get<2>(tickets[i]);
    long double summed_cost = std::get<1>(tickets[i]);
    if (travel_time <= summed_time && summed_cost < smallest_cost) {
      smallest_cost = summed_cost;
      best_tickets.clear();
      best_tickets.push_back(std::get<0>(tickets[i]));
      continue;
    }
    for (size_t j = i; j < tickets.size(); j++) {
      summed_cost = std::get<1>(tickets[i]) + std::get<1>(tickets[j]);
      summed_time = std::get<2>(tickets[i]) + std::get<2>(tickets[j]);

      if (travel_time <= summed_time && summed_cost < smallest_cost) {
        smallest_cost = summed_cost;
        best_tickets.clear();
        best_tickets.push_back(std::get<0>(tickets[i]));
        best_tickets.push_back(std::get<0>(tickets[j]));
        continue;
      }
      for (size_t k = j; k < tickets.size(); k++) {
        summed_cost = std::get<1>(tickets[i]) + std::get<1>(tickets[j]) + std::get<1>(tickets[k]);
        summed_time = std::get<2>(tickets[i]) + std::get<2>(tickets[j]) + std::get<2>(tickets[k]);
        if (travel_time <= summed_time && summed_cost < smallest_cost) {
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
  // nie da sie kupić biletów na zadany okres czasu
  if (best_tickets.size() == 0) throw std::invalid_argument("");
  return best_tickets;
}

// current_travel zawiera opis niepustej podróży
// funkcja time_of_connection zwraca nieujemną liczbę reprezentującą
// sumaryczny czas podróży
// w przypadku niepoprawnych danych rzuca wyjątek invalid_argument
// w przypadku gdy na pewnym przystanku trzeba czekać, rzuca wyjątek domain_error
// z nazwą przystanku jako argumentem
int time_of_connection(const std::unordered_map<long long, stops>& courses,
                       const travel& current_travel) {
  time_point current_time, start_point;

  for (size_t i = 0; i < current_travel.size() - 1; i++) {
    long long course_number = current_travel[i].second;

    if (!(courses.find(course_number) != courses.end())) {
      // nie istnieje kurs o zadanym numerze
      throw std::invalid_argument("");
    }
    stops current_course = courses.at(course_number);

    std::string current_stop_name = current_travel[i].first;
    std::string next_stop_name = current_travel[i + 1].first;

    if (current_course.find(current_stop_name) == current_course.end() ||
        current_course.find(next_stop_name) == current_course.end()) {
      // w podanym kursie nie ma takiego przystanku
      throw std::invalid_argument("");
    }

    time_point departure_from_stop = current_course[current_stop_name];
    time_point arrival_for_the_next_stop = current_course[next_stop_name];

    if (departure_from_stop > arrival_for_the_next_stop) {
      throw std::invalid_argument("");
    }

    if (i == 0) {
      current_time = arrival_for_the_next_stop;
      start_point = departure_from_stop;
    } else {
      if (current_time == departure_from_stop) {
        current_time = arrival_for_the_next_stop;
      } else if (current_time < departure_from_stop) {
        // trzeba czekać na tym przystanku
        throw std::domain_error(current_stop_name);
      } else {
        // tramwaj odjechał przd naszym przyjazdem
        throw std::invalid_argument("");
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

std::pair<long long, stops> parse_course(const std::smatch& sm) {
  long long number = std::stoll(sm[1].str());
  stops course;

  std::vector<std::string> v;
  string_split(sm[2].str(), v);

  time_point previous = opening_time;
  // Czasu muszą być rosnące, a 5:55 musi działać
  previous.second -= 1;
  for (int i = 0; i < (int)v.size() - 1; i += 2) {
    std::vector<std::string> time;
    string_split(v[i], time, ':');

    time_point tp = std::make_pair(std::stoi(time[0]), std::stoi(time[1]));
    if (tp <= previous) {
      throw std::invalid_argument("Invalid time - earlier than on a previous stop");
    }
    previous = tp;

    std::string stop = v[i + 1];
    if (course.count(stop)) {
      throw std::invalid_argument("This stop already exists in course");
    }
    course[stop] = tp;
  }

  if (previous > closing_time) {
    throw std::invalid_argument("Invalid time - after closing time");
  }

  return std::make_pair(number, course);
}

bool try_add_course(std::unordered_map<long long, stops>& courses, const std::smatch& sm) {
  std::pair<long long, stops> course;
  try {
    course = parse_course(sm);
  } catch (const std::logic_error& e) {
    return false;
  }
  if (courses.count(course.first)) {
    return false;
  }
  courses[course.first] = course.second;
  return true;
}

single_ticket parse_ticket(const std::smatch& sm) {
  std::string name = sm[1].str();
  // Część całkowita i ułamkowa w osobnych grupach, by łatwo było je złączyć
  long long price = std::stoll(sm[2].str() + sm[3].str());
  long long duration = std::stoll(sm[4].str());
  return std::make_tuple(name, price, duration);
}

bool try_add_ticket(std::vector<single_ticket>& tickets, const std::smatch& sm) {
  single_ticket ticket;
  try {
    ticket = parse_ticket(sm);
  } catch (const std::logic_error& e) {
    return false;
  }
  for (auto t : tickets) {
    if (std::get<0>(t) == std::get<0>(ticket)) {
      return false;
    }
  }
  tickets.push_back(ticket);
  return true;
}

travel parse_travel_query(const std::smatch& sm) {
  std::string travel_str = sm[1].str();
  std::vector<std::string> args;
  string_split(travel_str, args);
  travel result;

  for (size_t i = 0; i < args.size(); i += 2) {
    long long nr;
    if (i != args.size() - 1) {
      nr = std::stoll(args[i + 1]);
    } else {
      nr = -1;
    }
    std::string name = args[i];
    result.push_back(std::make_pair(name, nr));
  }

  return result;
}

bool try_perform_query(const std::vector<single_ticket>& tickets,
                       const std::unordered_map<long long, stops>& courses, const std::smatch& sm,
                       int& ticket_sum) {
  travel query;
  int time;
  std::vector<std::string> solution;

  try {
    query = parse_travel_query(sm);
  } catch (const std::logic_error& e) {
    return false;
  }

  try {
    time = time_of_connection(courses, query);
  } catch (const std::invalid_argument& e) {
    return false;
  } catch (const std::domain_error& e) {
    std::cout << ":-( " << e.what() << "\n";
    return true;
  }

  try {
    solution = buy_tickets(tickets, time);
  } catch (const std::invalid_argument& e) {
    std::cout << ":-|\n";
    return true;
  }

  std::cout << "!";
  for (size_t i = 0; i < solution.size(); i++) {
    std::cout << " " << solution[i];
    if (i != solution.size() - 1) std::cout << ";";
  }
  std::cout << "\n";

  ticket_sum += solution.size();

  return true;
}

int main() {
  // courses: numer linii -> rozkład
  std::unordered_map<long long, stops> courses;
  std::vector<single_ticket> tickets;
  int ticket_sum = 0;

  std::string line;
  size_t line_number = 0;

  while (std::getline(std::cin, line)) {
    line_number++;
    if (line.length() == 0) continue;

    std::smatch sm;
    if (std::regex_match(line, sm, add_course_regex)) {
      if (!try_add_course(courses, sm)) {
        print_error(line_number, line);
      }
    } else if (std::regex_match(line, sm, add_ticket_regex)) {
      if (!try_add_ticket(tickets, sm)) {
        print_error(line_number, line);
      }
    } else if (std::regex_match(line, sm, travel_query_regex)) {
      if (!try_perform_query(tickets, courses, sm, ticket_sum)) {
        print_error(line_number, line);
      }

    } else {
      print_error(line_number, line);
    }
  }

  std::cout << ticket_sum << "\n";
}
