#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>

using std::function;

class FooBar {

/*
    count -> 1
    mutex
    condition variable
*/

private:
    int n;
    std::mutex turn_mutex_;
    std::condition_variable turn_notify_;
    bool turn_;

public:
    FooBar(int n) {
        turn_ = 0;
        this->n = n;
    }

    void foo(function<void()> printFoo) {
        
        for (int i = 0; i < n; i++) {
            std::unique_lock lck(turn_mutex_);

            turn_notify_.wait(lck, [&] {
                return turn_ == 0;
            });
        	// printFoo() outputs "foo". Do not change or remove this line.
        	printFoo();

            turn_ = 1;
            turn_notify_.notify_one();
        }
    }

    void bar(function<void()> printBar) {
        
        for (int i = 0; i < n; i++) {
            std::unique_lock lck(turn_mutex_);

            turn_notify_.wait(lck, [&] {
                return turn_ == 1;
            });
        	// printBar() outputs "bar". Do not change or remove this line.
        	printBar();

            turn_ = 0;
            turn_notify_.notify_one();
        }
    }
};

void PrintFoo() {
    std::cout << "Foo";
}

void PrintBar() {
    std::cout << "Bar" << std::endl;
}

int main() {
    FooBar f(5);

    std::thread foo_thread(&FooBar::foo, &f, PrintFoo);
    std::thread bar_thread(&FooBar::bar, &f, PrintBar);

    foo_thread.join();
    bar_thread.join();

    return 0;
}