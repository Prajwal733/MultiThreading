#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

int shared_counter;
std::mutex shared_counter_mtx;

void IncrementCounter() {
    std::lock_guard lck(shared_counter_mtx);
    shared_counter++;
}

int main() {
    std::vector<std::thread> threads;

    for(int i=0;i<100000;i++) {
        threads.push_back(std::thread(IncrementCounter));
    }

    for(std::thread& thread: threads) {
        thread.join();
    }

    std::cout << "Final value of shared counter: " << shared_counter << std::endl;

    return 0;
}