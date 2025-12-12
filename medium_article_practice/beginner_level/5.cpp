#include <iostream>
#include <thread>
#include <future>
// #include <packaged_task> 
#include <vector> 

/*
    create a thread that calculates the sum of an array 

    1. promises and futures 
    2. packaged task 
    3. async programming 
*/

void CalculateAndSetPromise(std::vector<int>& arr, std::promise<int>& p) {
    int sum = 0;

    for(int& x: arr) {
        sum += x;
    }

    p.set_value(sum);
}


int CalculateSum(std::vector<int>& arr) {
    int sum = 0;

    for(int& x: arr) {
        sum += x;
    }

    return sum;
}

int main() {
    // Promise + future 
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::vector<int> v = {1, 2, 3};
    std::thread t(&CalculateAndSetPromise, std::ref(v), std::ref(p));

    t.join();
    std::cout << f.get() << std::endl;

    // packaged_task 
    std::packaged_task<int(std::vector<int>&)> task(CalculateSum);
    std::thread t1(std::ref(task), std::ref(v));

    f = task.get_future();

    std::cout << f.get() << std::endl;
    t1.join();

    // async 
    f = std::async(&CalculateSum, std::ref(v));

    std::cout << f.get() << std::endl;

    return 0;
}