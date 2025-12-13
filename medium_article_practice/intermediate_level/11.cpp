#include <iostream>
#include <thread>

class Temp {
public:
    void PrintSomething() {
        std::cout << "Printing something to the screen" << std::endl;
    }
};

int main() {
    Temp t;

    std::thread t1 = std::thread(&Temp::PrintSomething, std::ref(t));

    t1.join();

    return 0;
}
