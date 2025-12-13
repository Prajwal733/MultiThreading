#include <iostream>
#include <mutex>
#include <condition_variable>

/*
    Reader - Writer Problem 

    -> 1:100 write to read ratio (assumption)
    -> when there is thread waiting to write, all the other threads should wait, other
       wise it can cause write starvation 

*/

class ReaderWriter {
public:
    void Write(std::string new_value) {
        {
            std::lock_guard lck(write_mutex_);
            write_waiting_count_++;
        }

        {
            std::lock_guard lck(resource_mutex_);
            resource_ = new_value;
        }

        {
            std::lock_guard lck(write_mutex_);
            write_waiting_count_--;
            if (write_waiting_count_ == 0)
                write_signal_.notify_all();
        }
    }

    std::string Read() {
        {
            std::unique_lock lck(write_mutex_);
            write_signal_.wait(lck, [&] {
                return write_waiting_count_ == 0;
            });
        }

        std::shared_lock lck(resource_mutex_);
        return resource_;
    }

private:
    std::string resource_;
    std::shared_mutex resource_mutex_;

    int write_waiting_count_;
    std::mutex write_mutex_;
    std::condition_variable write_signal_;
};