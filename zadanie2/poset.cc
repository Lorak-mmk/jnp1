#include "poset.h"

#define relationMap std::unordered_map<std::string, std::unordered_set<std::string>>
#define relationsSet std::unordered_map<unsigned long, relationMap>

/**
 * W unordered_set<std::string> trzymamy klucze większe od danego
 */
relationsSet biggerThan;

/**
 * W unordered_set<std::string> trzymamy klucze mniejsze od danego
 */
relationsSet smallerThan;
std::deque<unsigned long> releasedIds;
unsigned long lastId;

unsigned long poset_new(void){

    unsigned long newId;
    if(releasedIds.empty()){

        if(activeIds.empty())   lastId = 0;
        else                    lastId++;               // Uwaga na max ilosc zbiorow?

        newId = lastId;

    }else{

        newId = releasedIds.front();
        releasedIds.pop_front();

    }

    biggerThan.emplace(newId, relationMap());
    smallerThan.emplace(newId, relationMap());

    return newId;
}

void poset_delete(unsigned long id){

    smallerThan.erase(id);
    auto check = biggerThan.find(id);
    if(check != biggerThan.end()){
        biggerThan.erase(id);
        releasedIds.push_front(id);
    }

}

size_t poset_size(unsigned long id){

    auto pSet = biggerThan.find(id);
    if(pSet != biggerThan.end()) return (*pSet).second.size();
    else return 0;

}

bool poset_insert(unsigned long id, char const *value){
    
    std::string val = std::string(value);
    auto checkId = biggerThan.find(id);
    if(checkId != biggerThan.end()){
        
        auto checkElement = (*checkId).second.find(val);
        if(checkElement == (*checkId).second.end()){

            (*checkId).second.emplace(val, std::unordered_set<std::string>());
            checkId = smallerThan.find(id);
            (*checkId).second.emplace(val, std::unordered_set<std::string>());
            return true;

        }else return false;

    }else return false;
}

bool poset_remove(unsigned long id, char const *value){

    std::string val = std::string(value);
    auto checkId = biggerThan.find(id);
    if(checkId != biggerThan.end()){

        auto checkElement = (*checkId).second.find(val);
        if(checkElement != (*checkId).second.end()){

            // Usuwam relacje wieksze niz
            for(auto i = (*checkElement).second.begin(); i != (*checkElement).second.end(); i++){
                auto helper = (*checkId).second.find(*i);
                (*helper).second.erase(val);
            }

            auto checkIdSmaller = smallerThan.find(id);
            checkElement = (*checkIdSmaller).second.find(val);

            // Usuwam relacje mniejsze niz
            for(auto i = (*checkElement).second.begin(); i != (*checkElement).second.end(); i++){
                auto helper = (*checkIdSmaller).second.find(*i);
                (*helper).second.erase(val);
            }

            // Usuwam element
            (*checkId).second.erase(val);
            (*checkIdSmaller).second.erase(val);

            return true;

        }else return false;

    }else return false;
}

bool poset_add(unsigned long id, char const *value1, char const *value2){

    std::string val1 = std::string(value1);
    std::string val2 = std::string(value2);
    
}