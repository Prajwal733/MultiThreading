#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>

class MutexBasedCounter {
public:
    MutexBasedCounter() : value_(0) {
    }

    void Increment() {
        std::lock_guard lck(value_mutex_);
        value_++;
    }

    int GetValue() {
        std::lock_guard lck(value_mutex_);
        return value_;
    }

private:
    int value_;
    std::mutex value_mutex_;
};

class AtomicBasedCounter {
public:
     AtomicBasedCounter() : value_(0) {
     }

     void Increment() {
        value_++;
     }

     int GetValue() {
        return value_;
     }
    
private:    
    std::atomic<int> value_;
};

template<typename T> 
std::pair<double, int> ExecuteAndReturnSum(int num_threads, int times,T& obj) {
    std::vector<std::thread> threads;

    auto t1 = std::chrono::steady_clock::now();
    for(int i=0;i<num_threads;i++) {
        threads.push_back(std::thread([&obj, &times] {
            for (int i=0;i<times;i++)
                obj.Increment();
        }));
    }

    for(std::thread& thread: threads) {
        thread.join();
    }
    auto t2 = std::chrono::steady_clock::now();

    std::chrono::duration<double> duration = t2 - t1;

    return {duration.count(), obj.GetValue()};
}

int main() {

    for(int i=1;i<=10;i++) {
        AtomicBasedCounter abs;

        std::cout << i << std::endl;
        auto k = ExecuteAndReturnSum(i, 10000000, abs);
        std::cout << k.first << " " << k.second << std::endl;

        MutexBasedCounter mbs;
        auto k1 = ExecuteAndReturnSum(i, 10000000, mbs);
        std::cout << k1.first << " " << k1.second << std::endl;

        std::cout << std::endl;
    }
    

    return 0;
}

/*
Output:

1
0.0876572 10000000
0.182547 10000000

2
2
0.174478 20000000
1.39392 20000000

3
0.369047 30000000
1.45683 30000000

4
0.436463 40000000
2.62539 40000000

5
0.584523 50000000
3.62169 50000000

6
0.765498 60000000
4.63123 60000000

7
0.926536 70000000
5.7364 70000000

8
1.16594 80000000
6.68164 80000000

9
1.23719 90000000
7.37937 90000000

10
1.4421 100000000
8.4947 100000000

*/
