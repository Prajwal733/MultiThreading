#include <iostream>
#include <thread>
#include <vector>

void DoSomeUselessWork() {
    std::cout << "Doing some useless work" << std::endl;
}

int main() {
    std::vector<std::thread> v;

    for(int i=0;i<10;i++) {
        std::thread t(DoSomeUselessWork);

        // v.push_back(t) will give error because copy operator of std::thread is blocked
        v.push_back(std::move(t));
    }

    // for(std::thread thread: v) will give error because of copy operator
    for(std::thread& thread: v) {
        thread.join();
    }

    return 0;
}