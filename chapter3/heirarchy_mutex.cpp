#include <iostream>
#include <thread>
#include <mutex>

class HeirarchicalMutex {
public:
    HeirarchicalMutex(long value) {
        mutex_value = value;
    }

    void lock() {
        if (thread_current_value < mutex_value) {
            std::cout << "Invalid operation" << std::endl;
            exit(0);
        }

        mtx.lock();
        previous_value = thread_current_value;
        thread_current_value = mutex_value;
    }

    void unlock() {
        if (thread_current_value != mutex_value) {
            std::cout << "Not a valid operation" << std::endl;
            exit(0);
        }

        mtx.unlock();
        thread_current_value = previous_value;
    }

    void try_lock() {
        // TODO(prajwalts): implement this 
    }


private:
    static thread_local long thread_current_value;
    std::mutex mtx;
    long previous_value;
    long mutex_value;
};

 thread_local long
    HeirarchicalMutex::thread_current_value(100000);

HeirarchicalMutex high_mutex(100);
HeirarchicalMutex low_mutex(10);



void LowAfterHigh() {
    std::lock_guard lck(low_mutex);
}

void HighLevel() {
    std::lock_guard lck1(high_mutex);
    LowAfterHigh();
}

int main() {
    LowAfterHigh();

    return 0;
}