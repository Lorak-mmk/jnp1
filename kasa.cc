#include <algorithm>
#include <cfloat>
#include <iostream>
#include <regex>
#include <stdlib>
#include <string>

using time_point = pair<int, int>;
using stops = unordered_map<string, time_point>;
using single_ticket = tuple<string, double, time_point>;
// każdy element to odpowiednio nazwa przystanku, numer kursu
using travel = vector<string, int>;

static const std::regex add_course_regex("(-?\\d+)((?: (?:\\d{1,2}:\\d{2}) (?:[a-zA-Z_^]+))+)");
static const std::regex add_ticket_regex("([a-zA-Z ]+) (\\d+\\.\\d{2}) ((?!0)\\d+)");
static const std::regex travel_query_regex("\\? ([a-zA-Z_^]+(?: -?\\d+ [a-zA-Z_^]+)*)");

unordered_map<int, stops> courses;
vector<single_tickets> tickets;

int time_distance(time_point a, time_point b) {
  return abs(a.first - b.first) * 60 + abs(a.second - b.second);
}

bool equal_times(time_point a, time_point b) {
  if (a.first == b.first && a.second == b.second) return true;
  return false;
}

// travel_time - czas, na który mamy kupić bilety podany w minutach
vector<string> buy_tickets(int travel_time) {
  long double smallest_cost = LDBL_MAX;
  vector<string> best_tickets;

  for (int i = 0; i < tickets; i++) {
    time_point summed_time = get<2> tickets[i];
    long double summed_cost = get<1> tickets[i];
    if (ok_czas_biletu && summed_cost < smallest_cost) {
      smallest_cost = summed_cost;
      best_tickets.clear();
      best_tickets.push_back(get<0> tickets[i]);
      continue;
    }
    for (int j = i; j < tickets; j++) {
      for (int k = j; k < tickets; k++) {
        if (ok_czas biletow && summed_cost < smallest_cost) {
          best_tickets.clear();
          best_tickets.push_back(get<0> tickets[0]);
          best_tickers.push_back smallest_cost = summed_cost;
        }
      }
    }
  }
}

// current_travel zawiera opis niepustej podróży
// funkcja time_of_connection zwraca nieujemną liczbę reprezentującą 
// sumaryczny czas podróży
// lub liczbę ujemną równą current_travel.size(), gdy dane wejściowe są
// niepoprawne inna liczba ujemna reprezentuje indeks przystanku, na którym
// trzeba czekać
int time_of_connection(travel current_travel) {
  time_point current_time, start_point;

  for (int i = 0; i < current_travel.size() - 1; i++) {
    int course_number = current_travel[i].second;
    stops course = find(courses, course_number);

    if (course == courses.end()) {
      // nie istnieje kurs o zadanym numerze
      return current_travel.size() * (-1);
    }

    int current_stop_name = current_travel[i].first;
    int next_stop_name = current_travel[i + 1].first;

    time_point departure_from_stop = find(course, current_stop_name);
    time_point arrival_for_the_stop = find(course, next_stop_name);

    if (departure_from_stop == coures.end() || arrival_for_the_stop == courses.end()) {
      // w podanym kursie nie ma takiego przystanku
      return current_travel.size() * (-1);
    }

    if (i == 0) {
      current_time = departure_from_stop;
      start_point = departure_from_stop;
    } else {
      if (!equal_times(current_time, departure_from_stop)) {
        // trzeba czekać na tym przystanku
        return -i;
      } else {
        current_time = arrival_for_the_stop;
      }
    }
  }

  return current_time - start_point;
}

void print_error(size_t line_number, std::string line) {
  std::cerr << "Error in line " << line_number << ": " << line << "\n";
}

std::pair<int, stops> parse_course(std::smatch sm) {
    int number = std::stoi(sm[1].str()); //TODO: handle exceptions to catch numbers that are too big
    
}

single_ticket parse_ticket(std::smatch sm) {
  std::string name = sm[1].str();
  double price = std::stod(sm[2].str());
  time_point tp = std::make_pair(0, 0);  // TODO: Let's discuss form of time_point first. Also, if
                                         // time_point ends up being from std::chrono, then this
                                         // should be duration from std::chrono
  return std::make_tuple(name, price, tp);
}

travel parse_travel_query(std::smatch sm) {
    
}

int main(int argc, char* argv[]) {
  std::string line;
  size_t line_number = 0;

  while (std::getline(std::cin, line)) {
    line_number++;
    if (line.length() == 0) continue;

    std::smatch sm;
    if (std::regex_match(line, sm, add_course_regex)) {
      std::pair<int, stops> course = parse_course(sm);
    } else if (std::regex_match(line, sm, add_ticket_regex)) {
      single_ticket ticket = parse_ticket(sm);
    } else if (std::regex_match(line, sm, travel_query_regex)) {
      travel query = parse_travel_query(sm);
    } else {
      print_error(line_number, line);
    }
  }
}
