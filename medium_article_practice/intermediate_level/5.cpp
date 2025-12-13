#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <vector>

/*
    Philosopher -> 
        acquire -> i , (i+1) lock
        eat 
        release the lock
        sleep for some time 

    5 locks
    stop signal 
    counter for all 

*/

class DiningPhilosopher {
public:
    DiningPhilosopher() {
        complete_process_ = false;
        for (int i=0;i<5;i++) {
            total_chance_[i] = 0;
        }
    }
    void StartProcess() {
        std::vector<std::thread> philosophers;

        for(int i=0;i<5;i++) {
            std::cout << "Spawning the ith philosopher " << sizeof(complete_process_)<< std::endl;
            philosophers.push_back(std::thread(&DiningPhilosopher::Philosopher, this, i));
        }
        
        {
            std::unique_lock lck(complete_process_mutex_);
            complete_signal_.wait(lck, [&] {
                return complete_process_;
            });
        }

        for(std::thread& phil: philosophers) {
            phil.join();
        }

        for(int i=0;i<5;i++) {
            std::cout << total_chance_[i] << std::endl;
        }

        return;
    }

    void SignalEnd() {
        std::lock_guard lck(complete_process_mutex_);
        complete_process_ = true;

        complete_signal_.notify_one();
    }

    void Philosopher(int i) {
        std::cout << "Philosopher entered " << std::endl;
        while (true) {
            {
                std::lock_guard lck(complete_process_mutex_);
                if (complete_process_) {
                    std::cout << "Philosopher saying bye " << i << std::endl;
                    return;
                }
            }

            std::cout << "Philosopher " << i << " is ready to eat" << std::endl;
            {
                int first_lock = i, second_lock = (i+1) % 5;
                std::lock(spoons_[first_lock], spoons_[second_lock]);
                std::lock_guard lck1(spoons_[first_lock], std::adopt_lock);
                std::lock_guard lck2(spoons_[second_lock], std::adopt_lock);
                std::cout << "Philosopher " << i << " is eating" << std::endl;
            }   

            total_chance_[i]++;
            std::cout << "Philosopher " << i << " is going to sleep" << std::endl;
            sleep(1);
        }
    }

private:
    std::mutex spoons_[5];
    int total_chance_[5];
    std::mutex complete_process_mutex_;
    bool complete_process_;
    std::condition_variable complete_signal_;
};

int main() {
    DiningPhilosopher dp;
    std::thread t1(&DiningPhilosopher::StartProcess, std::ref(dp));

    sleep(10);

    dp.SignalEnd();

    t1.join();

    return 0;
}