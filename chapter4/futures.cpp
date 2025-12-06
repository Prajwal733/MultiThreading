#include <iostream>
#include <future>
#include <unistd.h>

int HeavyAddition(int a, int b) {
    std::cout << "Heavy addition called " << std::endl;

    return a + b;
}

int main() {
    std::future<int> value_future = std::async(HeavyAddition, 1, 2);

    auto run_in_new_thread = std::async(std::launch::async, HeavyAddition, 2, 3); // runs in new thread

    // will not be run, unless wait or get called on the instance 
    auto run_when_wait_or_get_called = std::async(std::launch::deferred, HeavyAddition, 5, 6); // runs only when get or wait called 

    int ans = value_future.get();

    run_when_wait_or_get_called.wait();
    std::cout << ans << std::endl;

    return 0;
}