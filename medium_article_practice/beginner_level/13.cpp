#include <iostream>
#include <thread>

int main() {
    std::thread t([]{
        for(int i=0;i<100;i++) {
            std::cout << i << " hello from Prajwal" << std::endl;
        }
    });

    t.join();

    return 0;
}