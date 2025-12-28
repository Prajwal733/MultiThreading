/*
    Spin lock using std::atomic_flag 
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class SpinLock {
public:
    SpinLock() : lck(ATOMIC_FLAG_INIT) {
    }

    void Lock() {
        while (lck.test_and_set());
    }

    void Unlock() {
        lck.clear();
    }

private:
    std::atomic_flag lck;
};

int count;
SpinLock sp;

void ThreadWork() {
    for (int i=1;i<=10000000;i++) {
        sp.Lock();
        count++;
        sp.Unlock();
    }
}

int main() {
    std::vector<std::thread> threads;

    for (int i=0;i<7;i++) {
        threads.push_back(std::thread(&ThreadWork));
    }

    for (std::thread& thread: threads) {
        thread.join();
    }

    std::cout << count << std::endl;
    return 0;
}
