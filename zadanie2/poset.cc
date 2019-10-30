#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <cstring>

#include "poset.h"

#ifdef NDEBUG
static const bool DEBUG_ENABLED = false;
#else
static const bool DEBUG_ENABLED = true;
#endif

#define DEBUG(fmt, ...)                                   \
  do {                                                    \
    if (DEBUG_ENABLED) fprintf(stderr, fmt, __VA_ARGS__); \
  } while (0)

using element_id_t = int64_t;
using poset_element_t = std::unordered_set<element_id_t>;
using poset_t = std::unordered_map<std::string_view, element_id_t>;
template <typename T> using id_counter_t = std::tuple<std::deque<T>, T>;

std::unordered_map<unsigned long, poset_t *> posets;
std::unordered_map<element_id_t, poset_element_t*> elements;

id_counter_t<unsigned long> poset_counter;
id_counter_t<element_id_t> element_counter;

template <typename T>
T get_new_id(id_counter_t<T>& counter) {
  std::deque<T>& que = std::get<0>(counter);
  T& ctr = std::get<1>(counter);
  if (que.empty()) {
    return ctr++;
  } else {
    unsigned long newId = que.front();
    que.pop_front();
    return newId;
  }
}

template<typename T>
void free_id(id_counter_t<T>& counter, T id) { std::get<0>(counter).emplace_back(id); }

void clear_poset(poset_t *poset) {}

unsigned long jnp1::poset_new(void) {
  DEBUG("%s()\n", __func__);
  unsigned long newId = get_new_id(poset_counter);
  posets.emplace(newId, new poset_t());

  DEBUG("%s: poset %lu created\n", __func__, newId);
  return newId;
}

void jnp1::poset_delete(unsigned long id) {
  try {
    poset_t *poset = posets.at(id);
    clear_poset(poset);
    posets.erase(id);
    free_id(poset_counter, id);
  } catch (const std::out_of_range &e) {
    return;
  }
}

size_t jnp1::poset_size(unsigned long id) {
  try {
    poset_t *poset = posets.at(id);
    return poset->size();
  } catch (const std::out_of_range &e) {
    return 0;
  }
}

bool jnp1::poset_insert(unsigned long id, char const *value) {
  DEBUG("%s(%lu, \"%s\")\n", __func__, id, value);

  if (value == nullptr) {
    DEBUG("%s: invalid value (NULL)\n", __func__);
    return false;
  }

  poset_t* poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    DEBUG("%s: invalid id (%lu)\n", __func__, id);
    return false;
  }

  char* d = new char[strlen(value) + 1];
  strcpy(d, value);

  std::string_view s1 = std::string_view(d);
  if(poset->find(s1) != poset->end()){
    DEBUG("%s: invalid value (%s)\n", __func__, d);
    return false;
  }

  // TODO zamiana strcpy(?)
  char* data = new char[strlen(value) + 1];
  strcpy(data, value);
  s1 = std::string_view(data);

  element_id_t newId = get_new_id(element_counter);
  (*poset)[s1] = newId;
  elements[newId] = new poset_element_t();

  DEBUG("%s: poset %lu, element \"%s\" inserted\n", __func__, id, data);
  return true;
}

bool jnp1::poset_remove(unsigned long id, char const *value) {
  /*
  std::string val = std::string(value);
  auto checkId = biggerThan.find(id);
  if (checkId != biggerThan.end()) {
    auto checkElement = (*checkId).second.find(val);
    if (checkElement != (*checkId).second.end()) {
      // Usuwam relacje wieksze niz
      for (auto i = (*checkElement).second.begin(); i != (*checkElement).second.end(); i++) {
        auto helper = (*checkId).second.find(*i);
        (*helper).second.erase(val);
      }

      auto checkIdSmaller = smallerThan.find(id);
      checkElement = (*checkIdSmaller).second.find(val);

      // Usuwam relacje mniejsze niz
      for (auto i = (*checkElement).second.begin(); i != (*checkElement).second.end(); i++) {
        auto helper = (*checkIdSmaller).second.find(*i);
        (*helper).second.erase(val);
      }

      // Usuwam element
      (*checkId).second.erase(val);
      (*checkIdSmaller).second.erase(val);

      return true;

    } else
      return false;

  } else
    return false;
  */
}

bool jnp1::poset_add(unsigned long id, char const *value1, char const *value2) {
  /*
  std::string val1 = std::string(value1);
  std::string val2 = std::string(value2);

  if (strcmp(val1.c_str(), val2.c_str()) != 0) {
    auto biggerId = biggerThan.find(id);
    if (biggerId != biggerThan.end()) {
      auto checkVal1 = (*biggerId).second.find(val1);
      auto checkVal2 = (*biggerId).second.find(val2);

      if (checkVal1 != (*biggerId).second.end() && checkVal2 != (*biggerId).second.end()) {
        auto smallerId = smallerThan.find(id);

        // Dodaj value1 jako poprzednik value2, oraz jako poprzednik dla wszystkich nastepnikow
        // value2
        auto smallerMap = (*smallerId).second.find(val2);
        for (auto i = (*smallerMap).second.begin(); i != (*smallerMap).second.end(); i++) {
          auto current = (*smallerId).second.find((*i));
          (*current).second.insert(val1);
        }
        (*smallerMap).second.insert(val1);

        // Dodaj value2 jako nastepnik value1, oraz jako nastepnik dla wszystkich poprzednikow
        // value1
        for (auto i = (*checkVal1).second.begin(); i != (*checkVal1).second.end(); i++) {
          auto current = (*biggerId).second.find(*i);
          (*current).second.insert(val2);
        }
        (*checkVal2).second.insert(val2);

        return true;

      } else
        return false;

    } else
      return false;
  } else
    return false;
  */
}

bool jnp1::poset_del(unsigned long id, char const *value1, char const *value2) {
  /*
  std::string val1 = std::string(value1);
  std::string val2 = std::string(value2);

  if (strcmp(val1.c_str(), val2.c_str()) != 0) {
    auto biggerId = biggerThan.find(id);
    if (biggerId != biggerThan.end()) {
      auto checkVal1 = (*biggerId).second.find(val1);
      auto checkVal2 = (*biggerId).second.find(val2);

      if (checkVal1 != (*biggerId).second.end() && checkVal2 != (*biggerId).second.end()) {
        auto smallerId = smallerThan.find(id);

        // Usun value1 jako poprzednik value2, oraz jako poprzednik dla wszystkich nastepnikow
        // value2
        auto smallerMap = (*smallerId).second.find(val2);
        for (auto i = (*smallerMap).second.begin(); i != (*smallerMap).second.end(); i++) {
          auto current = (*smallerId).second.find((*i));
          (*current).second.erase(val1);
        }
        (*smallerMap).second.erase(val1);

        // Usun value2 jako nastepnik value1, oraz jako nastepnik dla wszystkich poprzednikow value1
        for (auto i = (*checkVal1).second.begin(); i != (*checkVal1).second.end(); i++) {
          auto current = (*biggerId).second.find(*i);
          (*current).second.erase(val2);
        }
        (*checkVal2).second.erase(val2);

        return true;

      } else
        return false;

    } else
      return false;
  } else
    return false;
  */
}

bool jnp1::poset_test(unsigned long id, char const *value1, char const *value2) {
  DEBUG("%s(%lu, \"%s\", \"%s\")\n", __func__, id, value1, value2);
  poset_t* poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    DEBUG("%s: poset %lu does not exist\n", __func__, id);
    return false;
  }

  // TODO zamiana kopiowania (?)
  char* d1 = new char[strlen(value1) + 1];
  strcpy(d1, value1);
  char* d2 = new char[strlen(value2) + 1];
  strcpy(d2, value2);
  std::string_view s1 = std::string_view(d1);
  std::string_view s2 = std::string_view(d2);

  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);

  if (elem1 == poset->end() || elem2 == poset->end()) {
    DEBUG("%s: element \"%s\" or \"%s\" does not exist\n", __func__, d1, d2);
    return false;
  }

  if (elem1->second == elem2->second) {
    DEBUG("%s: poset %lu, relation (\"%s\", \"%s\") exists\n", __func__, id, d1, d2);
    return true;
  }

  poset_element_t* base = elements.at(elem1->second);

  if (base->find(elem2->second) == base->end()) {
    DEBUG("%s: poset %lu, relation (\"%s\", \"%s\") exists\n", __func__, id, d1, d2);
    return true;
  } else {
    DEBUG("%s: poset %lu, relation (\"%s\", \"%s\") does not exist\n", __func__, id, d1, d2);
    return false;
  }

  return base->find(elem2->second) == base->end();
}

void jnp1::poset_clear(unsigned long id) {
  DEBUG("%s(%lu)\n", __func__, id);
  try {
    poset_t *poset = posets.at(id);
    clear_poset(poset);
    DEBUG("%s: poset %lu cleared\n", __func__, id);
  } catch (const std::out_of_range &e) {
    DEBUG("%s: poset %lu does not exist\n", __func__, id);
    return;
  }
}
