#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

using std::function;

void ReleaseHydrogen() {
    std::cout << "H";
}

void ReleaseOxygen() {
    std::cout << "O";
}

class H2O {
private:
    struct Molecule {
        short hydrogen;
        bool oxygen;
    };

    Molecule current_;
    std::mutex current_mutex_;
    std::condition_variable current_notify_;

    void CheckAndResetMolecule() {
        if (current_.hydrogen == 2 and current_.oxygen == 1) {
            current_ = {0, 0};
        }
    }
public:
    H2O() {
        current_ = {0, 0};
    }

    void hydrogen(function<void()> releaseHydrogen) {
        std::unique_lock lck(current_mutex_);

        current_notify_.wait(lck, [&] {
            return current_.hydrogen < 2;
        });
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        current_.hydrogen++;

        CheckAndResetMolecule();

        current_notify_.notify_all();
    }

    void oxygen(function<void()> releaseOxygen) {
        std::unique_lock lck(current_mutex_);

        current_notify_.wait(lck, [&] {
            return current_.oxygen < 1;
        });
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        current_.oxygen = 1;

        CheckAndResetMolecule();

        current_notify_.notify_all();
    }
};

int main() {
    H2O water;

    std::vector<std::thread> all_threads;

    for(int i=0;i<6;i++) {
        std::thread hydrogen1(&H2O::hydrogen, &water, ReleaseHydrogen);
        all_threads.push_back(std::move(hydrogen1));
    }

    for(int i=0;i<3;i++) {
        std::thread oxygen1(&H2O::oxygen, &water, ReleaseOxygen);
        all_threads.push_back(std::move(oxygen1));
    }

    for(std::thread& thread: all_threads) {
        thread.join();
    }

    return 0;
}