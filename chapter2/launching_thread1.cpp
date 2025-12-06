#include <iostream>
#include <thread>

void DoWork() {
    std::cout << "Doing work in the function " << std::endl;
}

int main() {
    // 1. Through function
    std::thread t1(DoWork);

    // 2. lambda function
    std::thread t2([]{
        std::cout << "Doing work in lambda function" << std::endl;
    });

    // Don't forget to join or detach
    t1.join();
    t2.join();

    return 0;
}