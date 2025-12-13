#include <iostream>
#include <unistd.h>
#include <thread>
#include <atomic>

class CountDownTimer {
public:
    CountDownTimer(int count) : count_(count) {
        stop_flag_ = false;
    }

    void StartTimer() {
        std::thread t( [this]() {
            std::cout << "Starting the Timer" << std::endl;
            while (count_ > 0 && stop_flag_ == false) {
                std::cout << "Time: " << count_ << std::endl;
                sleep(1);
                count_--;
            }
       });

       counter_thread_ = std::move(t);
    }

    ~CountDownTimer() {
        std::cout << "Destructor called" << std::endl;
        stop_flag_ = true;
        counter_thread_.join();
        std::cout << "Thread joined" << std::endl;
    }

private:    
    int count_;
    std::atomic<bool> stop_flag_;
    std::thread counter_thread_;
};

int main() {
    CountDownTimer c(3);

    c.StartTimer();
    sleep(2);

    std::cout << "Hello from Main" << std::endl;

    return 0;
}
