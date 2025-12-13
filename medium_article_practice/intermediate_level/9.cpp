#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> counter;

void IncrementCounter(int times) {
    for(int i=0;i<times;i++) {
        counter++;
    }
}

int main() {
    int num_threads = 1000;
    std::vector<std::thread> threads;
    int times = 100000;

    for(int i=0;i<num_threads;i++) {
        threads.push_back(std::thread(&IncrementCounter, times));
    }

    for(std::thread& thread: threads) {
        thread.join();
    }

    std::cout << "Final value of the counter: " << counter << std::endl;

    return 0;
}
