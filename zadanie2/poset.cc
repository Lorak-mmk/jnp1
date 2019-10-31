#include <string.h>
#include <queue>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "poset.h"

#ifdef NDEBUG
static const bool DEBUG_DISABLED = true;
#else
static const bool DEBUG_DISABLED = false;
#endif

#define DEBUG \
    if (DEBUG_DISABLED) {} \
    else std::cerr

using element_id_t = int64_t;
using elements_t = std::unordered_set<element_id_t>;
// std::tuple<smaller elements, bigger elements>
using poset_element_t =
    std::tuple<elements_t, elements_t>;
// Elements of all posets are being held on one map, and have unique ids.
// Poset just maps strings to ids of elements.
using poset_t = std::unordered_map<std::string_view, element_id_t>;
// A mechanism to assign new ids to posets and elements, without much risk of overflow
template <typename T>
using id_counter_t = std::tuple<std::queue<T>, T>;

// Local helper functions.
namespace {
  std::unordered_map<unsigned long, poset_t *> posets;
  std::unordered_map<element_id_t, poset_element_t *> elements;

  id_counter_t<unsigned long> poset_counter;
  id_counter_t<element_id_t> element_counter;

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

  template <typename T>
  void free_id(id_counter_t<T> &counter, T id) {
    std::get<0>(counter).emplace(id);
  }

  void delete_underlying_string(const std::string_view &s) { free((void *)s.data()); }

  void poset_clear_elements(poset_t *poset) {
    for (auto &it : *poset) {
      delete_underlying_string(it.first);
    }
    poset->clear();
  }

  bool poset_test_relation(element_id_t id1, element_id_t id2) {
    if(id1 == id2) return true;
    
    poset_element_t *base = elements.at(id1);
    return std::get<1>(*base).find(id2) != std::get<1>(*base).end();
  }
  
  template<typename T>
  std::unordered_set<T> get_set_diff(const std::unordered_set<T>& main, const std::unordered_set<T>& to_remove) {
    std::unordered_set<T> result;
    
    for(const auto& elem : main) {
      if(to_remove.count(elem) == 0){
        result.insert(elem);
      }
    }
    
    return result;
  }
}

unsigned long jnp1::poset_new(void) {
  //DEBUG("%s()\n", __func__);
  unsigned long newId = get_new_id(poset_counter);
  printf("%lu\n", newId);
  posets.emplace(newId, new poset_t());

  //DEBUG("%s: poset %lu created\n", __func__, newId);
  return newId;
}

void jnp1::poset_delete(unsigned long id) {
  try {
    poset_t *poset = posets.at(id);
    poset_clear_elements(poset);
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
  //DEBUG("%s(%lu, \"%s\")\n", __func__, id, value);

  if (value == nullptr) {
    //DEBUG("%s: invalid value (NULL)\n", __func__);
    return false;
  }

  poset_t *poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    //DEBUG("%s: invalid id (%lu)\n", __func__, id);
    return false;
  }

  std::string_view s = std::string_view(value);
  if (poset->find(s) != poset->end()) {
    //DEBUG("%s: invalid value (%s)\n", __func__, value);
    return false;
  }

  char *data = strdup(value);
  s = std::string_view(data);

  element_id_t newId = get_new_id(element_counter);
  (*poset)[s] = newId;
  elements[newId] = new poset_element_t();

  //DEBUG("%s: poset %lu, element \"%s\" inserted\n", __func__, id, data);
  return true;
}

bool jnp1::poset_remove(unsigned long id, char const *value) {
  //DEBUG("%s(%lu, \"%s\")\n", __func__, id, value);

  if (value == nullptr) {
    //DEBUG("%s: invalid value (NULL)\n", __func__);
    return false;
  }

  poset_t *poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    //DEBUG("%s: invalid id (%lu)\n", __func__, id);
    return false;
  }

  auto it = poset->find(std::string_view(value));
  if (it == poset->end()) {
    return false;
  }

  element_id_t id_to_remove = it->second;

  for (const auto &elem : std::get<0>(*elements[id_to_remove])) {
    std::get<1>(*elements[elem]).erase(id_to_remove);
  }

  for (const auto &elem : std::get<1>(*elements[id_to_remove])) {
    std::get<0>(*elements[elem]).erase(id_to_remove);
  }

  delete_underlying_string(it->first);
  free_id(element_counter, id_to_remove);

  poset->erase(it);
  elements.erase(id_to_remove);

  return true;
}

bool jnp1::poset_add(unsigned long id, char const *value1, char const *value2) {
  //DEBUG("%s(%lu, \"%s\", \"%s\")\n", __func__, id, value1, value2);

  if (value1 == nullptr || value2 == nullptr) {
    //DEBUG("%s: invalid value (NULL)\n", __func__);
    return false;
  }

  poset_t *poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    //DEBUG("%s: invalid id (%lu)\n", __func__, id);
    return false;
  }
  
  std::string_view s1 = std::string_view(value1);
  std::string_view s2 = std::string_view(value2);
  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);
  
  if(elem1 == poset->end() || elem2 == poset->end()) {
    return false;
  }
  
  if(poset_test_relation(elem1->second, elem2->second) || poset_test_relation(elem2->second, elem1->second)){
    return false;
  }
  
  poset_element_t* p_elem1 = elements[elem1->second];
  poset_element_t* p_elem2 = elements[elem2->second];
  
  elements_t to_insert_bigger = get_set_diff<element_id_t>(std::get<1>(*p_elem2), std::get<1>(*p_elem1));  
  std::get<1>(*p_elem1).insert(to_insert_bigger.begin(), to_insert_bigger.end());
  for(const auto& elem : std::get<0>(*p_elem1)){
    std::get<1>(*elements[elem]).insert(to_insert_bigger.begin(), to_insert_bigger.end());
  }
  
  elements_t to_insert_smaller = get_set_diff<element_id_t>(std::get<0>(*p_elem1), std::get<0>(*p_elem2));
  std::get<0>(*p_elem2).insert(to_insert_smaller.begin(), to_insert_smaller.end());
  for(const auto& elem : std::get<1>(*p_elem2)){
    std::get<0>(*elements[elem]).insert(to_insert_smaller.begin(), to_insert_smaller.end());
  }
  
  return true;
}

bool jnp1::poset_del(unsigned long id, char const *value1, char const *value2) {
  //DEBUG("%s(%lu, \"%s\", \"%s\")\n", __func__, id, value1, value2);

  if (value1 == nullptr || value2 == nullptr) {
    //DEBUG("%s: invalid value (NULL)\n", __func__);
    return false;
  }

  poset_t *poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    //DEBUG("%s: invalid id (%lu)\n", __func__, id);
    return false;
  }
  
  std::string_view s1 = std::string_view(value1);
  std::string_view s2 = std::string_view(value2);
  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);
  
  if(elem1 == poset->end() || elem2 == poset->end()) {
    return false;
  }
  
  if(!poset_test_relation(elem1->second, elem2->second) || elem1->second == elem2->second){
    return false;
  }
  
  for(const auto& elem : std::get<1>(*elements[elem1->second])) {
    if(std::get<1>(*elements[elem]).count(elem2->second)) {
      return false;
    }
  }
  
  std::get<1>(*elements[elem1->second]).erase(elem2->second);
  std::get<0>(*elements[elem2->second]).erase(elem1->second);
  
  return true;

}

bool jnp1::poset_test(unsigned long id, char const *value1, char const *value2) {
  //DEBUG("%s(%lu, \"%s\", \"%s\")\n", __func__, id, value1, value2);
  
  if (value1 == nullptr || value2 == nullptr) {
    //DEBUG("%s: invalid value (NULL)\n", __func__);
    return false;
  }
  
  poset_t *poset;
  try {
    poset = posets.at(id);
  } catch (const std::out_of_range &e) {
    //DEBUG("%s: poset %lu does not exist\n", __func__, id);
    return false;
  }

  std::string_view s1 = std::string_view(value1);
  std::string_view s2 = std::string_view(value2);
  auto elem1 = poset->find(s1);
  auto elem2 = poset->find(s2);

  if (elem1 == poset->end() || elem2 == poset->end()) {
    //DEBUG("%s: element \"%s\" or \"%s\" does not exist\n", __func__, s1.data(), s2.data());
    return false;
  }

  if (elem1->second == elem2->second) {
    //DEBUG("%s: poset %lu, relation (\"%s\", \"%s\") exists\n", __func__, id, s1.data(), s2.data());
    return true;
  }

  if (poset_test_relation(elem1->second, elem2->second)) {
    //DEBUG("%s: poset %lu, relation (\"%s\", \"%s\") exists\n", __func__, id, s1.data(), s2.data());
    return true;
  } else {
    //DEBUG("%s: poset %lu, relation (\"%s\", \"%s\") does not exist\n", __func__, id, s1.data(), s2.data());
    return false;
  }
}

void jnp1::poset_clear(unsigned long id) {
  //DEBUG("%s(%lu)\n", __func__, id);
  try {
    poset_t *poset = posets.at(id);
    poset_clear_elements(poset);
    //DEBUG("%s: poset %lu cleared\n", __func__, id);
  } catch (const std::out_of_range &e) {
    //DEBUG("%s: poset %lu does not exist\n", __func__, id);
    return;
  }
}
