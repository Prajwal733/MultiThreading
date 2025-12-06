#include <iostream>
#include <thread>

void DoWork() {
    // Getting id of the current thread
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << "Do\n";
}

int main() {
    // indicates the number of threads that can truly run in parallel
    std::cout << std::thread::hardware_concurrency() << std::endl; 

    std::thread t(DoWork);

    // to get thread id from std::thread instance
    std::cout << t.get_id() << std::endl;

    std::thread::id id = t.get_id();

    std::cout << id << std::endl;
    
    t.join();
    return 0;
}