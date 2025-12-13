#include <iostream>
#include <thread>
#include <future>
#include <unistd.h>

/*
    two types of launch policies

    1. std::launch::async
    2. std::launch::deferred 

*/

int GetFactorial(int n) {
    std::cout << "Factorial starated execution" << std::endl;
    int ans = 1;
    for(int i=2;i<=n;i++) {
        ans = ans * i;
    }

    return ans;
}

int main() {
    std::future<int> f = std::async(std::launch::async, &GetFactorial, 5);

    sleep(1);
    std::cout << "Main continues the execution" << std::endl;
    std::cout << f.get() << std::endl;

    return 0;
}
