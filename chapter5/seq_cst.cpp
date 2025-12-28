#include <iostream>
#include <atomic>
#include <thread>

using std::memory_order_seq_cst;

std::atomic<bool> x(false), y(false);
int z;

void WriteXThenY() {
    x.store(true, memory_order_seq_cst);
    y.store(true, memory_order_seq_cst);
}

void Read() {
    while (!y.load(memory_order_seq_cst));
    if (x.load(memory_order_seq_cst))
        z++;
}

int main() {
    std::thread one(&WriteXThenY);
    std::thread two(&Read);

    one.join();
    two.join();

    // z should be equal to 1, since the x,y store operations cannot be reordered. 
    std::cout << z << std::endl;

    return 0;
}
