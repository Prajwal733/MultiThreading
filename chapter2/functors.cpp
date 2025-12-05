#include <iostream>
#include <thread>

class DoWork {
public:
    DoWork() = default;
    DoWork(DoWork& d) {
        std::cout << "Copy constructor called" << std::endl;
    }

    void operator() () {
        std::cout << "Doing work in functor" << std::endl;
    }
};

int main() {
    DoWork d;

    // This will copy d before executing in thread context
    std::thread t1(d);

    t1.join();

    return 0;
}