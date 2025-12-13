#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>

class ThreadWork {
public:
    ThreadWork() {
        stop_flag_ = false;
    }

    void operator() () {
        while(!stop_flag_) {
            std::cout << "Hello from the background thread" << std::endl;
        }
    }

    void SignalStop() {
        stop_flag_ = true;
    }

private:
    std::atomic<bool> stop_flag_;
};

int main() {
    ThreadWork t;
    std::thread t1(std::ref(t));
    t1.detach();

    sleep(2);
    t.SignalStop();

    return 0;
}
