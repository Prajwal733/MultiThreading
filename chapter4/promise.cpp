#include <iostream>
#include <future>
#include <thread>

void Add(std::promise<int>& p, int a, int b) {
    p.set_value(a + b);
}

int main() {
    std::promise<int> p;
    std::shared_future<int> f = p.get_future().share();

    // shared_future, both of them can retrieve the result
    std::shared_future<int> f2 = f;

    std::thread worker(Add, std::ref(p), 1, 2);

    worker.detach();

    std::cout << f.get() << std::endl;
    std::cout << f2.get() << std::endl;

    return 0;
}
