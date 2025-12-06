#include <iostream>
#include <thread>
#include <cassert>

void DoSomeWork() {
    int count = 0;

    for(int i=0;i<1000;i++) {
        count += (5 * i);
    }

    std::cout << "Final count of DoSomeWork is " << count << std::endl;
}

int main() {
    std::thread t1(DoSomeWork);
    std::thread t2(DoSomeWork);

    std::thread t3;

    t3 = std::move(t1);

    // After transferring ownership to t3, t1 should not be joinable 
    assert(!t1.joinable());

    t1 = std::move(t2);

    // This terminates the program as we are trying to assign two threads to single variable
    t1 = std::move(t3);

    t1.join();
    t3.join();
}