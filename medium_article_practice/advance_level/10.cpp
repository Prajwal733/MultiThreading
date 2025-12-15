#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cmath>
using namespace std;

int kBatch = 1024;
class PrimeNumbers {
public:
    PrimeNumbers(int available_threads) {
        available_threads_ = available_threads;
    }

    vector<bool> GetPrimeNumbers(int start, int end) {
        int current_start  = start;
        mutex current_mutex;

        vector<bool> is_prime(end-start+1, false);
        vector<thread> threads;

        for(int i=0;i<available_threads_;i++) {
            threads.push_back(thread([&current_start, &current_mutex, &start, &end, &is_prime, this]{
                while (true) {
                    int local_start, local_end;
                    {
                        std::lock_guard lck(current_mutex);
                        local_start = current_start;
                        current_start += kBatch;
                    }

                    if (local_start > end) {
                        return;
                    }

                    local_end = min(end, local_start + kBatch);

                    for (int num = local_start; num <= local_end; num++) {
                        if (IsPrime(num)) {
                            is_prime[num - start] = true;
                        }
                    }
                }
            }));
        }

        for(thread& thread: threads) {
            thread.join();
        }

        return is_prime;
    }

private:
    bool IsPrime(int num) {
        for(int i=2;i<=sqrt(num);i++) {
            if (num%i == 0) {
                return false;
            }
        }

        return true;
    }

    int available_threads_;
};

int main() {
    PrimeNumbers p(10);

    vector<bool> ans = p.GetPrimeNumbers(1, 100000);

    for(int i=0;i<ans.size();i++) {
        if (ans[i]) {
            cout << i + 1 << endl;
        }
    }

    return 0;
}
