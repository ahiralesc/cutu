/* ==========================================================================
Copyright 2017 The CUTU Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
============================================================================= */


#ifndef _BARRIER__
#define _BARRIER__

#include <mutex>
#include <condition_variable>


class Barrier {
private:
    std::mutex bmutex;
    std::condition_variable cv;
    std::size_t count;
    std::size_t n;
    std::size_t generation;

public:
    explicit Barrier(unsigned sz = 0) : count{sz}, n{sz}, generation{0}{};

    void reset(std::size_t sz){
        count = sz;
        n = sz;
        generation = 0;
    }

    void wait() {
        std::unique_lock<std::mutex> lck{bmutex};
        auto gen = generation;
        if(--count == 0) {
            generation++;
            count = n;
            cv.notify_all();
        }else {
            cv.wait(lck,[this, gen]{ return gen != generation;});
        }
    }

    size_t getGeneration(){
        return generation;
    }
};

#endif
