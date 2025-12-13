#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class AsyncLogger {
public:
    AsyncLogger() : stop_signal_(false), worker_thread_(std::thread(&AsyncLogger::Work, this)) {
    }

    void Log(int severity, std::string msg) {
        std::lock_guard lck(logs_mutex_);

        if (logs_.size() == kQueueLimit) {
            return;
        }

        logs_.push({severity, msg});
        logs_signal_.notify_one();
    }

    ~AsyncLogger() {
        std::cout << "Destructor called" << std::endl;
        {
            std::lock_guard lck(logs_mutex_);
            stop_signal_ = true;
            logs_signal_.notify_one();
        }
        worker_thread_.join();
    }

private:
    void Work() {
        std::cout << "Going to work" << std::endl;
        while (true) {
            std::unique_lock lck(logs_mutex_);

            logs_signal_.wait(lck, [&] {
                return logs_.size()  != 0 || stop_signal_;
            });

            if (logs_.size() == 0) {
                std::cout << "Received the stop signal" << std::endl;
                return;
            }

            std::pair<int, std::string> log;
            log = logs_.front();
            logs_.pop();
            lck.unlock();

            std::cout << log.first << " " << log.second << std::endl;
        }
    }

    int kQueueLimit = 100;
    std::queue<std::pair<int,std::string>> logs_;
    std::mutex logs_mutex_;
    std::condition_variable logs_signal_;

    std::thread worker_thread_;
    bool stop_signal_;
};

int main() {
    AsyncLogger logger;

    logger.Log(2, "message from main");
    
    logger.Log(3, "message from another main");

    return 0;
}
