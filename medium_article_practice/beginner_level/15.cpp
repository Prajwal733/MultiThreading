#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int shared_counter;
std::mutex shared_counter_mutex;

void IncrementNTimes(int n) {
    for(int i=0;i<n;i++) {
        std::lock_guard lck(shared_counter_mutex);
        shared_counter++;
    }
}

int main() {
    std::vector<std::thread> threads;

    for(int i=0;i<100;i++) {
        threads.push_back(std::thread(&IncrementNTimes, 1000000));
    }

    for(std::thread& thread: threads) {
        thread.join();
    }

    std::cout << "Value of shared counter: " << shared_counter << std::endl;

    return 0;
}