#include <string.h>
#include <iostream>
#include <queue>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "poset.h"

#ifdef NDEBUG
static const bool DEBUG_DISABLED = true;
#else
static const bool DEBUG_DISABLED = false;
#endif

#define DEBUG           \
  if (DEBUG_DISABLED) { \
  } else                \
    assert_stream_init(), std::cerr

using element_id_t = int64_t;
using elements_t = std::unordered_set<element_id_t>;
// std::tuple<smaller elements, bigger elements>
using poset_element_t = std::tuple<elements_t, elements_t>;
// Elements of all posets are being held on one map, and have unique ids.
// Poset just maps strings to ids of elements.
using poset_t = std::unordered_map<std::string_view, element_id_t>;
// A mechanism to assign new ids to posets and elements, without much risk of overflow
template <typename T>
using id_counter_t = std::tuple<std::queue<T>, T>;

// Local helper functions.
namespace {
// Wrapper for creating functions allowing to fix static order initialization.
// Won't work if 2 global vars have same type, so don't use it like that.
// You can work around it, by adding second template argument unsigned int N
// and passing to it for example __COUNTER__ macro or some other unique identifier.
template <typename T>
T &global_var() {
  static T *val = new T();
  return *val;
}

auto posets = global_var<std::unordered_map<unsigned long, poset_t *>>;
auto elements = global_var<std::unordered_map<element_id_t, poset_element_t *>>;
auto poset_counter = global_var<id_counter_t<unsigned long>>;
auto element_counter = global_var<id_counter_t<element_id_t>>;

const size_t SMALLER = 0;
const size_t BIGGER = 1;

// Code may be executed before main(), so we need to make sure that streams are available.
inline void assert_stream_init() {
  static std::ios_base::Init *INIT = new std::ios_base::Init();
  (void)INIT;
}

// Get new available id from counter.
// If there are freed ids available, will return one of them.
// If not, returns new id and increments internal counter.
template <typename T>
T get_new_id(id_counter_t<T> &counter) {
  std::queue<T> &que = std::get<0>(counter);
  T &ctr = std::get<1>(counter);
  if (que.empty()) {
    return ctr++;
  } else {
    unsigned long newId = que.front();
    que.pop();
    return newId;
  }
}

// Free id taken from counter.
template <typename T>
void free_id(id_counter_t<T> &counter, T id) {
  std::get<0>(counter).emplace(id);
}

// string_view doesn't destroy underlying char array upon destruction
// so we need to do it manually before clearing map with string_view keys.
void delete_underlying_string(const std::string_view &s) { free((void *)s.data()); }

poset_t *poset_try_get(unsigned long id) {
  auto it = posets().find(id);
  if (it == posets().end()) {
    return nullptr;
  }

  return it->second;
}

void poset_clear_elements(poset_t *poset) {
  for (auto &it : *poset) {
    delete_underlying_string(it.first);
    elements().erase(it.second);
  }
  poset->clear();
}

// Check if id1 preceeds id2.
// This is true, if id1 == id2 (because poset is reflexive),
// or id2 is in id1's successor list.
bool poset_test_relation(element_id_t id1, element_id_t id2) {
  if (id1 == id2) return true;

  poset_element_t *base = elements().at(id1);
  return std::get<BIGGER>(*base).find(id2) != std::get<BIGGER>(*base).end();
}

// Create new set, which is copy of main, but without elements that are also in to_remove.
template <typename T>
std::unordered_set<T> get_set_diff(const std::unordered_set<T> &main, const std::unordered_set<T> &to_remove) {
  std::unordered_set<T> result;

  for (const auto &elem : main) {
    if (to_remove.count(elem) == 0) {
      result.insert(elem);
    }
  }

  return result;
}

// Get string representation in case data is nullptr. Makes it easier to print debugs
inline const char *str(const char *data) { return data == nullptr ? "NULL" : data; }
}  // namespace

unsigned long jnp1::poset_new(void) {
  DEBUG << __func__ << "()" << std::endl;

  unsigned long newId = get_new_id(poset_counter());
  posets().emplace(newId, new poset_t());

  DEBUG << __func__ << ": poset " << newId << " created" << std::endl;
  return newId;
}

void jnp1::poset_delete(unsigned long id) {
  DEBUG << __func__ << "(" << id << ")" << std::endl;

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return;
  }

  poset_clear_elements(poset);
  posets().erase(id);
  free_id(poset_counter(), id);

  DEBUG << __func__ << ": poset " << id << " deleted" << std::endl;
}

size_t jnp1::poset_size(unsigned long id) {
  DEBUG << __func__ << "(" << id << ")" << std::endl;

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return 0;
  }

  DEBUG << __func__ << ": poset " << id << " contains " << poset->size() << " element(s)" << std::endl;
  return poset->size();
}

bool jnp1::poset_insert(unsigned long id, char const *value) {
  DEBUG << __func__ << "(" << id << ", \"" << str(value) << "\")" << std::endl;

  if (value == nullptr) {
    DEBUG << __func__ << ": invalid value (NULL)" << std::endl;
    return false;
  }

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return false;
  }

  std::string_view s = std::string_view(value);
  if (poset->find(s) != poset->end()) {
    DEBUG << __func__ << ": poset " << id << ", element \"" << s.data() << "\" already exists" << std::endl;
    return false;
  }

  char *data = strdup(value);
  s = std::string_view(data);
  element_id_t newId = get_new_id(element_counter());

  (*poset)[s] = newId;
  elements()[newId] = new poset_element_t();

  DEBUG << __func__ << ": poset " << id << ", element \"" << data << "\" inserted" << std::endl;
  return true;
}

bool jnp1::poset_remove(unsigned long id, char const *value) {
  DEBUG << __func__ << "(" << id << ", \"" << str(value) << "\")" << std::endl;

  if (value == nullptr) {
    DEBUG << __func__ << ": invalid value (NULL)" << std::endl;
    return false;
  }

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return false;
  }

  auto it = poset->find(std::string_view(value));
  if (it == poset->end()) {
    DEBUG << __func__ << ": poset " << id << ", element \"" << value << "\" does not exist" << std::endl;
    return false;
  }

  element_id_t id_to_remove = it->second;

  for (const auto &elem : std::get<SMALLER>(*elements()[id_to_remove])) {
    std::get<BIGGER>(*elements()[elem]).erase(id_to_remove);
  }

  for (const auto &elem : std::get<BIGGER>(*elements()[id_to_remove])) {
    std::get<SMALLER>(*elements()[elem]).erase(id_to_remove);
  }

  delete_underlying_string(it->first);
  poset->erase(it);
  elements().erase(id_to_remove);
  free_id(element_counter(), id_to_remove);

  DEBUG << __func__ << ": poset " << id << ", element \"" << value << "\" removed" << std::endl;
  return true;
}

bool jnp1::poset_add(unsigned long id, char const *value1, char const *value2) {
  DEBUG << __func__ << "(" << id << ", \"" << str(value1) << "\", \"" << str(value2) << "\")" << std::endl;

  if (value1 == nullptr || value2 == nullptr) {
    if (value1 == nullptr) {
      DEBUG << __func__ << ": invalid value1 (NULL)" << std::endl;
    }
    if (value2 == nullptr) {
      DEBUG << __func__ << ": invalid value2 (NULL)" << std::endl;
    }
    return false;
  }

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return false;
  }

  std::string_view s1 = std::string_view(value1);
  std::string_view s2 = std::string_view(value2);
  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);

  if (elem1 == poset->end() || elem2 == poset->end()) {
    DEBUG << __func__ << ": poset " << id << ", element \"" << s1.data() << "\" or \"" << s2.data()
          << "\" does not exist" << std::endl;
    return false;
  }

  if (poset_test_relation(elem1->second, elem2->second) || poset_test_relation(elem2->second, elem1->second)) {
    DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data()
          << "\") cannot be added" << std::endl;
    return false;
  }

  poset_element_t *p_elem1 = elements()[elem1->second];
  poset_element_t *p_elem2 = elements()[elem2->second];

  elements_t to_insert_bigger = get_set_diff<element_id_t>(std::get<BIGGER>(*p_elem2), std::get<BIGGER>(*p_elem1));
  to_insert_bigger.insert(elem2->second);
  std::get<BIGGER>(*p_elem1).insert(to_insert_bigger.begin(), to_insert_bigger.end());
  for (const auto &elem : std::get<SMALLER>(*p_elem1)) {
    std::get<BIGGER>(*elements()[elem]).insert(to_insert_bigger.begin(), to_insert_bigger.end());
  }

  elements_t to_insert_smaller = get_set_diff<element_id_t>(std::get<SMALLER>(*p_elem1), std::get<SMALLER>(*p_elem2));
  to_insert_smaller.insert(elem1->second);
  std::get<SMALLER>(*p_elem2).insert(to_insert_smaller.begin(), to_insert_smaller.end());
  for (const auto &elem : std::get<BIGGER>(*p_elem2)) {
    std::get<SMALLER>(*elements()[elem]).insert(to_insert_smaller.begin(), to_insert_smaller.end());
  }

  DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data() << "\") added"
        << std::endl;
  return true;
}

bool jnp1::poset_del(unsigned long id, char const *value1, char const *value2) {
  DEBUG << __func__ << "(" << id << ", \"" << str(value1) << "\", \"" << str(value2) << "\")" << std::endl;

  if (value1 == nullptr || value2 == nullptr) {
    if (value1 == nullptr) {
      DEBUG << __func__ << ": invalid value1 (NULL)" << std::endl;
    }
    if (value2 == nullptr) {
      DEBUG << __func__ << ": invalid value2 (NULL)" << std::endl;
    }
    return false;
  }

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return false;
  }

  std::string_view s1 = std::string_view(value1);
  std::string_view s2 = std::string_view(value2);
  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);

  if (elem1 == poset->end() || elem2 == poset->end()) {
    DEBUG << __func__ << ": poset " << id << ", element \"" << s1.data() << "\" or \"" << s2.data()
          << "\" does not exist" << std::endl;
    return false;
  }

  if (!poset_test_relation(elem1->second, elem2->second) || (elem1->second == elem2->second)) {
    DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data()
          << "\") cannot be deleted" << std::endl;
    return false;
  }

  for (const auto &elem : std::get<BIGGER>(*elements()[elem1->second])) {
    if (std::get<BIGGER>(*elements()[elem]).count(elem2->second)) {
      DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data()
            << "\") cannot be deleted" << std::endl;
      return false;
    }
  }

  std::get<BIGGER>(*elements()[elem1->second]).erase(elem2->second);
  std::get<SMALLER>(*elements()[elem2->second]).erase(elem1->second);

  DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data() << "\") deleted"
        << std::endl;
  return true;
}

bool jnp1::poset_test(unsigned long id, char const *value1, char const *value2) {
  DEBUG << __func__ << "(" << id << ", \"" << str(value1) << "\", \"" << str(value2) << "\")" << std::endl;

  if (value1 == nullptr || value2 == nullptr) {
    if (value1 == nullptr) {
      DEBUG << __func__ << ": invalid value1 (NULL)" << std::endl;
    }
    if (value2 == nullptr) {
      DEBUG << __func__ << ": invalid value2 (NULL)" << std::endl;
    }
    return false;
  }

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return false;
  }

  std::string_view s1 = std::string_view(value1);
  std::string_view s2 = std::string_view(value2);
  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);

  if (elem1 == poset->end() || elem2 == poset->end()) {
    DEBUG << __func__ << ": poset " << id << ", element \"" << s1.data() << "\" or \"" << s2.data()
          << "\" does not exist" << std::endl;
    return false;
  }

  if (elem1->second == elem2->second) {
    DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data() << "\") exists"
          << std::endl;
    return true;
  }

  bool result = poset_test_relation(elem1->second, elem2->second);

  DEBUG << __func__ << ": poset " << id << ", relation (\"" << s1.data() << "\", \"" << s2.data() << "\") "
        << (result ? "exists" : "does not exist") << std::endl;
  return result;
}

void jnp1::poset_clear(unsigned long id) {
  DEBUG << __func__ << "(" << id << ")" << std::endl;

  poset_t *poset = poset_try_get(id);
  if (poset == nullptr) {
    DEBUG << __func__ << ": poset " << id << " does not exist" << std::endl;
    return;
  }

  poset_clear_elements(poset);

  DEBUG << __func__ << ": poset " << id << " cleared" << std::endl;
}
