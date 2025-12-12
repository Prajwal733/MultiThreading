#include <iostream>
#include <thread>
#include <unistd.h>

int main() {
    std::thread join_thread([] {
        std::cout << "Hello from joining thread" << std::endl;
    });

    join_thread.join();

    std::thread detach_thread([] {
        sleep(1);
        std::cout << "Hello from the detach thread" << std::endl;
    });

    detach_thread.detach();
    return 0;
}