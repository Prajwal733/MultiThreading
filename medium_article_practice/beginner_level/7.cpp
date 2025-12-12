#include <iostream>
#include <thread>
#include <mutex>

int counter;
std::mutex counter_mutex;

void IncrementCounter() {
    for(int i=0;i<100;i++) {
        counter_mutex.lock();
        counter++;
        counter_mutex.unlock();
    }
}

int main() {
    std::thread t1(&IncrementCounter);
    std::thread t2(&IncrementCounter);

    t1.join();
    t2.join();

    std::cout << "Final value of the counter: " << counter << std::endl;

    return 0;
}