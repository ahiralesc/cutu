/* Copyright 2017 The CUTU Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

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
    
    void clear() {
        reg.clear();
        duplicates.clear();
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

    void clear() {
        duplicates.clear();
        v.clear(); 
    }

private:
    std::set<T> duplicates;
    std::vector<T> v;
};

#endif
