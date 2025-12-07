#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

using std::function;

void PrintNumber(int n) {
    std::cout << n;
}

class ZeroEvenOdd {
/*
  zero loops -> n times 
  odd loops -> 1 to ((n+1)/2)
  even loops -> 1 to (n/2)

  turn -> 1 to 2n  (mutex, condition_variable)
*/
private:
    int n;
    int turn_;
    std::mutex turn_mutex_;
    std::condition_variable turn_notify_;

public:
    ZeroEvenOdd(int n) {
        this->n = n;
        turn_ = 1;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        for(int i=0;i<n;i++) {
            std::unique_lock lck(turn_mutex_);

            // std::cout << "Value of turn " << turn_ << std::endl;
            turn_notify_.wait(lck, [&] {
                return turn_ % 2;
            });

            printNumber(0);

            turn_++;

            turn_notify_.notify_all();
        }
    }

    void even(function<void(int)> printNumber) {
        for(int i=1;i<=(n/2);i++) {
            std::unique_lock lck(turn_mutex_);

            turn_notify_.wait(lck, [&]{
                return (turn_ % 2 == 0) && ((turn_ / 2) % 2 == 0);
            });
            printNumber(2 * i);

            turn_++;

            turn_notify_.notify_all();
        }
    }

    void odd(function<void(int)> printNumber) {
        for(int i=1;i<=((n+1)/2);i++) {
            std::unique_lock lck(turn_mutex_);

            turn_notify_.wait(lck, [&]{
                return (turn_ % 2 == 0) && ((turn_ / 2) % 2);
            });
            printNumber(2 * i - 1);

            turn_++;

            turn_notify_.notify_all();
        }
    }
};

int main() {
    ZeroEvenOdd obj(8 /*7*/);

    std::thread zero(&ZeroEvenOdd::zero, &obj, PrintNumber);
    std::thread odd(&ZeroEvenOdd::odd, &obj, PrintNumber);
    std::thread even(&ZeroEvenOdd::even, &obj, PrintNumber);

    zero.join();
    odd.join();
    even.join();

    return 0;
}