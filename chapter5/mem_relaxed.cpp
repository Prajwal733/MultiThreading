#include <iostream>
#include <atomic>
#include <thread>

using std::memory_order_relaxed;

std::atomic<bool> x(false), y(false);
int z;

void WriteXThenY() {
    x.store(true, memory_order_relaxed);
    y.store(true, memory_order_relaxed);
}

void Read() {
    while (!y.load(memory_order_relaxed));
    if (x.load(memory_order_relaxed))
        z++;
}

int main() {
    for (int i=0;i<1000000;i++) {
        // init 
        x.store(false);
        y.store(false);
        z = 0;

        std::thread one(&WriteXThenY);
        std::thread two(&Read);

        one.join();
        two.join();

        // even though the load of y reads true and the store of x happens before the store of y. x
        // and y are different variables, so there are no ordering guarantees relating to the visibility
        // of values arising from operations on each.
        if (z == 0) {
            std::cout << "A case found" << std::endl;
        }
    }

    return 0;
}
