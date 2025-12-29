#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> sync(0);
std::atomic<int> data[5];

void Thread1() {
    data[0].store(0, std::memory_order_relaxed);
    data[1].store(1, std::memory_order_relaxed);
    data[2].store(2, std::memory_order_relaxed);
    data[3].store(3, std::memory_order_relaxed);
    data[4].store(4, std::memory_order_relaxed);

    sync.store(1, std::memory_order_release);
    // data and sync will be updated as a part of single batch.
}

void Thread2() {
    int expected = 1;
    // since it read-modify-write
    while (!sync.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
        std::cout << "expected " << expected << std::endl;
        expected = 1;
    }
}

void Thread3() {
    int expected = 2;
    while (!sync.compare_exchange_strong(expected, 3, std::memory_order_acquire)) {
        std::cout << "thread3 expected " << expected << std::endl;
        expected = 2;
    }

    for (int i=0;i<5;i++) {
        std::cout << data[i].load(std::memory_order_relaxed) << std::endl;
    }
}

int main() {
    std::thread t1(&Thread1);
    std::thread t2(&Thread2);
    std::thread t3(&Thread3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
