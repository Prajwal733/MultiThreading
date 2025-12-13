#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable> 
#include <queue>
#include <atomic>
#include <vector>

/*
    -> each producer will produce some fixed amount of values, atomic flag to track all
       the producers are completed or not. 
*/

class ProducerConsumer {
public:
    ProducerConsumer(int producer_count) : producer_count_(producer_count) {
    }

    void Producer(int id) {
        for(int i=0;i<100;i++) {
            std::lock_guard lck(value_mutex_);
            values_.push(id);

            value_signal_.notify_one();
        }

        producer_count_--;
        if (producer_count_ == 0) {
            production_complete_ = true;
            value_signal_.notify_all();
        }
    }

    void Consumer(int id) {
        while (true) {
            std::unique_lock lck(value_mutex_);

            value_signal_.wait(lck, [&] {
                return values_.size() != 0 or production_complete_;
            });

            if (values_.size() == 0) {
                value_signal_.notify_all();
                return;
            }

            int consume = values_.front();
            values_.pop();
            std::cout << "Consumed " << consume << " by " << id << std::endl;
        }
    }


private:
    std::queue<int> values_;
    std::mutex value_mutex_;
    std::condition_variable value_signal_;
    std::atomic<int> producer_count_;
    std::atomic<bool> production_complete_;
};

int main() {
    int producer_count = 2;
    ProducerConsumer pc(producer_count);

    std::vector<std::thread> all_threads;
    for(int i=0;i<producer_count;i++) {
        all_threads.push_back(std::thread(&ProducerConsumer::Producer, std::ref(pc), i + 1));
    }

    int consumer_count = 3;

    for(int i=0;i<consumer_count;i++) {
        all_threads.push_back(std::thread(&ProducerConsumer::Consumer, std::ref(pc), i+1));
    }

    for(std::thread& thread: all_threads) {
        thread.join();
    }

    return 0;
}
