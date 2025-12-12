#include <iostream>
#include <thread>

void DisplayMessage(std::string& msg) {
    std::cout << msg << std::endl;
}

int main() {
    std::string message = "Hello from Prajwal";

    std::thread t(&DisplayMessage, std::ref(message));

    t.join();

    return 0;
}