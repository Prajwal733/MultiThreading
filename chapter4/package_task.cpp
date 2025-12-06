#include <iostream>
#include <future>

std::string AddNumberToString(int a, std::string s) {
    return std::to_string(a) + s;
}

void DoWork(std::packaged_task<std::string(int, std::string)>& task) {
    task(1, " hi");
}

int main() {
    std::packaged_task<std::string(int, std::string)> pt(AddNumberToString);
    std::future<std::string> pt_future = pt.get_future();

    std::thread worker_thread(DoWork, std::ref(pt));
    worker_thread.detach();

    std::cout << pt_future.get() << std::endl;

    return 0;
}