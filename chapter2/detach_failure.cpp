#include <iostream>
#include <thread>

class DoWork {
public:
    DoWork(int& itr) {
        total_itr_ = &itr;
        std::cout << *total_itr_ << std::endl;
    }

    void operator() (int useless) {
        for(int i=0;i<*total_itr_;i++) {
            std::cout << *total_itr_ << std::endl;
        }
    }

private:
  int* total_itr_;
};

int main() {
    int r = 10000;
    DoWork d(r);

    // d will be copied and executed in thread 
    std::thread t1(d, 5);

    // allow t1 to run in background
    t1.detach();

    // adding some delay so that t1 starts executing
    for(int i=0;i<100;i++) {
        std::cout << "Hello from main thread" << std::endl;
    }
    
    // Program exits but t1 still has access to r, which is being destroyed, 
    // leading to undefined behaviour 
    return 0;
}