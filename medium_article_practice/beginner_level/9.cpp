#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1;
int counter;

void ThreadWork() {
    std::lock_guard lck(mtx1);

    counter++;
}

int main() {
    std::thread t(&ThreadWork);

    t.join();

    return 0;
}