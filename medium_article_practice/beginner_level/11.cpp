#include <iostream>
#include <thread>
#include <mutex>

class Counter {
public:
    Counter() {
        value_ = 0;
    }

    Counter(int value): value_(value){
    }
    
    void Increment() {
        std::lock_guard lck(mtx1_);
        value_++;
    }

    int GetValue() {
        std::lock_guard lck(mtx1_);
        return value_;
    }

private: 
    int value_;
    std::mutex mtx1_;
};

void ThreadWork(Counter& c) {
    for(int i=0;i<100000000;i++) {
        c.Increment();
    }
}

int main() {
    Counter c;
    std::thread t1(&ThreadWork, std::ref(c));
    std::thread t2(&ThreadWork, std::ref(c));

    t1.join();
    t2.join();

    std::cout << "Final value : " << c.GetValue() << std::endl;

    return 0;
}