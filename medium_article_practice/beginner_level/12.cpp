#include <iostream>
#include <thread>

void PrintOdd(int start, int end) {
    for(int i=start;i<=end;i++) {
        if (i%2) {
            std::cout << i << std::endl;
        }
    }
}

void PrintEven(int start, int end) {
    for(int i=start;i<=end;i++) {
        if (i%2 == 0) {
            std::cout << i << std::endl;
        }
    }
}

int main() {
    std::thread odd(&PrintOdd, 1, 100);
    std::thread even(&PrintEven, 1, 100);

    odd.join();
    even.join();

    return 0;
}