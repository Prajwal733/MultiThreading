#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

/*
    Demonstrate deadlock using std::mutex 
*/

std::mutex mtx1;
std::mutex mtx2;

void Thread1() {
    mtx1.lock();

    sleep(2);

    mtx2.lock();

    std::cout << "Thread1 progressing" << std::endl;

    mtx1.unlock();
    mtx2.unlock();
}

void Thread2() {
    mtx2.lock();

    mtx1.lock();

    std::cout << "Thread2 progressing" << std::endl;

    mtx1.unlock();

    mtx2.unlock();
}

int main() {
    std::thread t1(&Thread1);
    std::thread t2(&Thread2);

    t1.join();
    t2.join();

    std::cout << "Both the thread completed successfully" << std::endl;

    return 0;
}