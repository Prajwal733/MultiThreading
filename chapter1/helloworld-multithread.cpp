#include <iostream>
#include <thread>

void SayHello() {
    std:: cout << "Hello Concurrency!" << std::endl;
}

int main() {
    std::thread t(SayHello);

    t.join();
    return 0;
}