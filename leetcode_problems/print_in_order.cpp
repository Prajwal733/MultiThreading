#include <iostream>
#include <future>
#include <thread>
#include <functional>

using std::function;

class Foo {
/*
    1. condition variable + mutex
    2. promises and futures
*/

public:
    Foo() {
        std::cout << "Creating a new object" << std::endl;
        second_future_ = second_promise_.get_future();
        third_future_ = third_promise_.get_future();
    }

    void first(function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        second_promise_.set_value();
    }

    void second(function<void()> printSecond) {
        second_future_.get();
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        third_promise_.set_value();
    }

    void third(function<void()> printThird) {
        third_future_.get();
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }

private:
    std::promise<void> second_promise_, third_promise_;
    std::future<void> second_future_, third_future_;
};

void PrintFirst() {
    std::cout << "First" << std::endl;
}

void PrintSecond() {
    std::cout << "Second" << std::endl;
}

void PrintThird() {
    std::cout << "Third" << std::endl;
}

int main() {
    Foo f;
    std::thread thread3(&Foo::third, &f, PrintThird);
    std::thread thread2(&Foo::second, &f, PrintSecond);
    std::thread thread1(&Foo::first, &f, PrintFirst);

    thread3.join();
    thread2.join();
    thread1.join();

    return 0;
}