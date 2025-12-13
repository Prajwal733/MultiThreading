#include <iostream>
#include <thread>

thread_local int counter = 0;

int main() {
    std::thread t1 = std::thread([] {
        for(int i=0;i<10;i++) {
            counter++;
        }

        std::cout << "Counter inside thread1 is " << counter << std::endl;
    });

    std::cout << "Counter for main thread is " << counter << std::endl;

    t1.join();

    return 0;
}
