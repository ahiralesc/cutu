#ifndef __Online_Data_Stream_Duplicates_
#define __Online_Data_Stream_Duplicated_

#include <vector>
#include <set>
#include <cmath>
#include <map>

template <typename T>
class MapDataStream {
public:
    MapDataStream() {}
    std::set<T> getDuplicates() { return duplicates; }

    bool insert(T & element){
        bool st = reg.insert(std::pair<T,T>(element,element)).second;
        if(!st) duplicates.insert(element);
        return st;
    }

private:
    std::map<T,T> reg;  
    std::set<T> duplicates;
};


template <typename T>
class RangeDataStrem {
public:
    RangeDataStrem(std::vector<T> &_v) {v = std::move(_v); }
    std::set<T> getDuplicates() { return duplicates; }
    
    void findDuplicates() {
        for(unsigned i=0; i<v.size(); i++)
            if(v[std::abs(v[i])] >= 0)
                v[std::abs(v[i])] = -1 * v[std::abs(v[i])];
            else
                duplicates.insert(std::abs(v[i]));
    }

private:
    std::set<T> duplicates;
    std::vector<T> v;
};

#endif
