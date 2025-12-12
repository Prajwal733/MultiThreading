#include <iostream>
#include <thread>
#include <vector>

void CalculateAndPrintFactorial(int n) {
    int ans = 1;
    for(int i=2;i<=n;i++) {
        ans = ans * i;
    }

    std::string message = std::to_string(n);
    message += " -> ";
    message += std::to_string(ans);
    message += "\n";
    std::cout << message;
}

int main() {
    std::vector<std::thread> threads;

    for(int i=2;i<=10;i++) {
        threads.push_back(std::thread(&CalculateAndPrintFactorial, i));
    }

    for(std::thread& thread: threads) {
        thread.join();
    }

    return 0;
}