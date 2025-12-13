#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

std::mutex control;
std::condition_variable signal;

void FirstExecution() {
    std::lock_guard lck(control);
    std::cout << "First execution is now complete" << std::endl;

    signal.notify_one();
}

void SecondExecution() {
    std::unique_lock lck(control);

    signal.wait(lck);

    std::cout << "Second execution is now complete" << std::endl;
}

int main() {
    std::thread t2(&SecondExecution);
    sleep(10);
    std::thread t1(&FirstExecution);

    t1.join();
    t2.join();

    return 0;
}