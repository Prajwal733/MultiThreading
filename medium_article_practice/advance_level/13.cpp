#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h>
using namespace std;

class PipeLine {
public:
    PipeLine() {
        fetch_thread_ = std::move(thread(&PipeLine::Fetch, this));
        decode_thread_ = std::move(thread(&PipeLine::Decode, this));
        process_thread_ = std::move(thread(&PipeLine::Process, this));
    }

    void AddProcess(int id) {
        lock_guard lck(fetch_mtx_);
        pending_fetches_.push(id);
        fetch_signal_.notify_one();
    }

    ~PipeLine() {
        AddProcess(-1);

        fetch_thread_.join();
        decode_thread_.join();
        process_thread_.join();
    }

private:
    void Fetch() {
        while (true) {
            unique_lock lck(fetch_mtx_);

            fetch_signal_.wait(lck, [&]{
                return pending_fetches_.size() != 0;
            });

            int id = pending_fetches_.front();
            pending_fetches_.pop();
            if (id != -1) {
                cout << "Fetched the " << id << endl;
            }

            {
                lock_guard lck2(decode_mtx_);
                pending_decodes_.push(id);
                decode_signal_.notify_one();
            }
            

            if (id == -1) {
                // exit signal
                return;
            }
        }
    }

    void Decode() {
        while (true) {
            unique_lock lck(decode_mtx_);

            decode_signal_.wait(lck, [&]{
                return pending_decodes_.size() != 0;
            });

            int id = pending_decodes_.front();
            pending_decodes_.pop();
            if (id != -1) {
                cout << "Decode the " << id << endl;
            }

            {
                lock_guard lck2(process_mtx_);
                pending_process_.push(id);
                process_signal_.notify_one();
            }

            if (id == -1) {
                // exit signal
                return;
            }
        }
    }

    void Process() {
        while (true) {
            unique_lock lck(process_mtx_);

            process_signal_.wait(lck, [&]{
                return pending_process_.size() != 0;
            });

            int id = pending_process_.front();
            pending_process_.pop();
            if (id != -1) {
                cout << "Process the " << id << endl;
            }

            if (id == -1) {
                // exit signal
                return;
            }
        }
    }

    queue<int> pending_fetches_, pending_decodes_, pending_process_;
    mutex fetch_mtx_, decode_mtx_, process_mtx_;
    condition_variable fetch_signal_,decode_signal_, process_signal_;
    std::thread fetch_thread_, decode_thread_, process_thread_;
};

int main() {
    PipeLine p;

    for (int i=0;i<=10;i++) {
        p.AddProcess(i);
    }

    sleep(10);

    return 0;
}
