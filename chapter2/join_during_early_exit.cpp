#include <iostream>
#include <thread>

void SmallWork() {
    int count = 0;
    for(int i=0;i<100;i++) {
        count += (i * 5);
    }

    std::cout << "final value of count: " << count << std::endl;
}


class ThreadGuard {
public:
    ThreadGuard(std::thread* t) {
        std::cout << "Calling the constructor of the ThreadGuard" << std::endl;
        t_ = t;
    }

    ~ThreadGuard() {
        std::cout << "In the destructor of the thread" << std::endl;
        if(t_->joinable()) {
            std::cout << "Calling the join of the thread" << std::endl;
            t_->join();
        }
    }

private:
    std::thread* t_;
};

void ErrorWork() {
    std::thread t1(SmallWork);

    // some other operation, program terminates (aborts)
    if (true /*some condition*/) {
        return;
    }

    // some other work
    t1.join();
}

void CorrectWork() {
    std::thread t1(SmallWork);
    ThreadGuard tmp(&t1);

    // some other operation, program terminates (aborts)
    if (true /*some condition*/) {
        return;
    }

    // some other work
    t1.join();
}

int main() {
    std::thread t(CorrectWork);

    t.join();

    return 0;
}