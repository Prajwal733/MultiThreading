#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

std::mutex mtx1;
std::mutex mtx2;

void Thread1() {
    std::lock_guard lck1(mtx1);
    sleep(10);
    std::cout << "Holding first, waiting for the second" << std::endl;
    std::lock_guard lck2(mtx2);

    std::cout << "Thread1 is done with the work" << std::endl;
}

void Thread2() {
   std::lock_guard lck2(mtx2);
   sleep(10);
   std::cout << "Holding the second, waiting for the first" << std::endl;
   std::lock_guard lck1(mtx1);

   std::cout << "Thread2 is done with the work" << std::endl;
}

void GoodThread1() {
    std::scoped_lock lck(mtx1, mtx2);
    sleep(10);

    std::cout << "Good thread is doing some work" << std::endl;
}

void GoodThread2() {
    std::scoped_lock lck(mtx1, mtx2);
    sleep(10);

    std::cout << "Another Good thread is doing some work" << std::endl;
}

int main() {
    std::thread t1(GoodThread1);
    std::thread t2(GoodThread2);

    std::cout << "Waiting for the threads :(" << std::endl;
    t1.join();
    t2.join();

    return 0;
}