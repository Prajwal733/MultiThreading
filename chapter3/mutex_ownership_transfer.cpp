#include <iostream>
#include <thread>
#include <mutex>

int protected_data = 100;
std::mutex protected_data_mutex;

/*
// move and copy operator in lock_guard are blocked 
std::lock_guard<std::mutex> GetLockGuard() {
    std::lock_guard lck(protected_data_mutex);
    return lck;
}
*/

// unique_lock can be moved 
std::unique_lock<std::mutex> GetLockGuard() {
    /*
        can also used for deferred locking
        std::unique_lock lck(mtx, std::defer_lock);

        lck.lock();
    */
    std::unique_lock lck(protected_data_mutex);
    return lck;
}

int main() {
    std::unique_lock<std::mutex> lck(GetLockGuard());

    // Can be locked and unlocked manually as well.
    lck.unlock();

    std::cout << "Doing some operation" << std::endl;
    return 0;
}