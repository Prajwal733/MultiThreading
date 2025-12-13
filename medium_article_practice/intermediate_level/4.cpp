#include <iostream>
#include <future>
#include <unistd.h>
#include <vector>

int HeavyAddition(int a, int b) {
    sleep(2);
    return a + b;
}

int main() {
    std::vector<std::future<int>> futures;
    for(int i=0;i<10;i++) {
        int a = rand() % 100;
        int b = rand() % 100;
        std:: cout << a << " " << b << std::endl;
        futures.push_back(std::async(&HeavyAddition, a, b));
    }    

    for(std::future<int>& f: futures) {
        std::cout << f.get() << std::endl;
    }

    return 0;  
}