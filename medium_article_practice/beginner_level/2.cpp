#include <iostream>
#include <thread>

/*
  to get thread Ids 

  1. thread_obj.get_id()
  2. std::this_thread::get_id();
*/

void PrintHelloAndThreadId() {
    std::cout << "Hello from " << std::this_thread::get_id() << std::endl;
}

int main() {
    std::cout << "Id of main thread: " << std::this_thread::get_id() << std::endl;

    std::thread t1(&PrintHelloAndThreadId);
    std::thread t2(&PrintHelloAndThreadId);

    std::cout << "Thread id of 1st thread: " << t1.get_id() << std::endl;
    t1.join();
    t2.join();

    return 0;
}