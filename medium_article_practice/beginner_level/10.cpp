#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int counter;

void ThreadWork() {
    std::unique_lock lck(mtx);

    counter++;
}

int main() {
    std::thread t(&ThreadWork);

    t.join();

    return 0;
}