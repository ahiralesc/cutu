
class Barrier {
private:
    std::mutex bmutex;
    std::conditional_variable cv;
    std::size_t count;
    std::size_t n;
    std::size_t generation;

public:
    explicit Barrier(std::size_t sz) : count{sz}, n{sz}, generation{0}{};

    void wait() {
        std::unique_lock<std::mutex> lck{bmutex};
        if(--count == 0) {
            generation++;
            count = n;            
            cv.notify_all();
        }else
            cv.wait(lck,[this]{return count == 0 });
    }

    size_t getGeneration(){
        return generation;
    }
};
