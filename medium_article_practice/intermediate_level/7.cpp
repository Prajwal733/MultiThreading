#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <functional>

/*
    1. packaged tasks cannot be copied (std::move)
*/

class ThreadPool {
public:
    ThreadPool(int num_threads) : num_threads_(num_threads) {
        stop_signal_ = false;
    }

    void StartWrok() {
        std::vector<std::thread> threads;

        for(int i=0;i<num_threads_;i++) {
            threads.push_back(std::thread(&ThreadPool::ThreadWork, this));
        }

        {
            std::unique_lock lck(task_mutex_);
            task_signal_.wait(lck, [&] {
                return stop_signal_;
            });
        }

        for(std::thread& thread: threads) {
            thread.join();
        }
    }

    std::future<void> AddTask(std::function<void()> f) {
        std::packaged_task<void()> task(f);
        std::future<void> ft = task.get_future();

        std::lock_guard lck(task_mutex_);
        tasks_.push(std::move(task));
        task_signal_.notify_one();
        return ft;
    }

    void SignalStop() {
        std::lock_guard lck(task_mutex_);
        stop_signal_ = true;
        task_signal_.notify_all();
    }


private:
    void ThreadWork() {
        while(true) {
            std::unique_lock lck(task_mutex_);

            task_signal_.wait(lck, [&] {
                return tasks_.size() != 0 || stop_signal_;
            });

            if (tasks_.size() == 0) {
                task_signal_.notify_all();
                return;
            }

            auto task = std::move(tasks_.front());
            tasks_.pop();
            lck.unlock();

            task();
        }
    }

    int num_threads_;
    std::queue<std::packaged_task<void()>> tasks_;
    std::mutex task_mutex_;
    std::condition_variable task_signal_;
    bool stop_signal_;
};

int main() {
    ThreadPool tp(2);

    std::thread tp_thread(&ThreadPool::StartWrok, std::ref(tp));

    auto f1 = []() {
        // assume output is atomic 
        std::cout << "Hello from the first task" << std::endl;
    };
    tp.AddTask(f1);

    auto f2 = []() {
        std::cout << "Hello from the second task" << std::endl;
    };
    tp.AddTask(f2);

    tp.SignalStop();

    tp_thread.join();

    return 0;
}
