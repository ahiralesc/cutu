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
