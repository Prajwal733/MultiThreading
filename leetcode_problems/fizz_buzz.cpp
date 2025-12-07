#include <iostream>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>

using std::function;

void PrintFizz() {
    std::cout << "Fizz" << std::endl;
}

void PrintBuzz() {
    std::cout << "Buzz" << std::endl;
}

void PrintFizzBuzz() {
    std::cout << "FizzBuzz" << std::endl;
}

void PrintNumber(int i) {
    std::cout << i << std::endl;
}

class FizzBuzz {
private:
    int n;
    int turn_;
    std::mutex turn_mutex_;
    std::condition_variable turn_notify_;

    void DoWork(function<void(int)> work, function<bool()> pred) {
        while(turn_ <= n) {
            std::unique_lock lck(turn_mutex_);

            turn_notify_.wait(lck, [&] {
                return pred() || turn_ > n;
            });

            if (turn_ > n) {
                return;
            }

            work(turn_);

            turn_++;

            turn_notify_.notify_all();
        }
    }

public:
    FizzBuzz(int n) {
        this->n = n;
        turn_ = 1;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        DoWork([&](int i) {
            printFizz();
        }, [&]{
            return (turn_%3 == 0 && (turn_%5 != 0));
        });
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        // when (n%5 == 0 and (n%3 != 0))
        DoWork([&](int i) {
            printBuzz();
        }, [&]{
            return (turn_%3 != 0 && turn_ %5 == 0);
        });
    }

    // printFizzBuzz() outputs "fizzbuzz".
	void fizzbuzz(function<void()> printFizzBuzz) {
        // when (n%3 == 0 and n%5 == 0)
        DoWork([&](int i) {
            printFizzBuzz();
        }, [&]{
            return (turn_%3==0 && turn_%5 == 0);
        });
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        // if (n%3 != 0 and n%5 != 0)
        DoWork(printNumber, [&]{
            return (turn_ % 3 != 0 && turn_ % 5 != 0);
        });
    }
};

int main() {
    FizzBuzz f(15);

    std::thread fizz(&FizzBuzz::fizz, &f, PrintFizz);
    std::thread buzz(&FizzBuzz::buzz, &f, PrintBuzz);
    std::thread fizzBuzz(&FizzBuzz::fizzbuzz, &f, PrintFizzBuzz);
    std::thread number(&FizzBuzz::number, &f, PrintNumber);

    fizz.join();
    buzz.join();
    fizzBuzz.join();
    number.join();

    return 0;
}