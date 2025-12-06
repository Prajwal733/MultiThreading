#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

class ProducerConsumer {
public:
    void Producer() {
        for(int i=0;i<100;i++) {
            int new_data = i;
            std::lock_guard lck(data_mutex_);
            data_.push(new_data);
            data_notify_.notify_one();
        }
    }

    void Consumer(int consumer_number) {
        for(int i=0;i<10;i++) {
            std::unique_lock lck(data_mutex_);

            data_notify_.wait(lck, [&] {
                return data_.size() != 0;
            });

            int fetch = data_.front();
            data_.pop();
            lck.unlock();
            std::string output = std::to_string(consumer_number) + " consumed " + std::to_string(fetch) + "\n";
            std::cout << output;
        }   
    }

private:
    std::queue<int> data_;
    std::mutex data_mutex_;
    std::condition_variable data_notify_;
};

int main() {
    ProducerConsumer pc;
    std::thread producer(&ProducerConsumer::Producer, &pc);

    std::vector<std::thread> consumers;

    for(int i=0;i<10;i++) {
        consumers.push_back(std::thread(&ProducerConsumer::Consumer, &pc, i));
    }

    for(std::thread& t: consumers) {
        t.join();
    }
    producer.join();

    return 0;
}