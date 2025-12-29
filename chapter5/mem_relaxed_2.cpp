#include <iostream>
#include <atomic>
#include <thread>

#define memory_order std::memory_order_relaxed

std::atomic<int> a(0), b(0), c(0);
std::atomic<bool> go(false);

struct values {
    int a, b, c;
};

values readvalues1[10], readvalues2[10], readvalues3[10], readvalues4[10], readvalues5[10];

void UpdateAndRead(std::atomic<int>* value_to, values* readvalues) {
    while (!go.load()) {
        std::this_thread::yield();
    }

    for (int i=0;i<10;i++) {
        readvalues[i].a = a.load(memory_order);
        readvalues[i].b = b.load(memory_order);
        readvalues[i].c = c.load(memory_order);

        value_to->store(i+1, memory_order);
        std::this_thread::yield();
    }
}

void Read(values* readvalues) {
    while (!go.load()) {
        std::this_thread::yield();
    }

    for (int i=0;i<10;i++) {
        readvalues[i].a = a.load(memory_order);
        readvalues[i].b = b.load(memory_order);
        readvalues[i].c = c.load(memory_order);

        std::this_thread::yield();
    }
}

void PrintArray(values* readvalues) {
    for (int i=0;i<10;i++) {
        std::cout << readvalues[i].a << " " << readvalues[i].b << " " << readvalues[i].c << std::endl;
    }

    std::cout << "End of one thread's output " << std::endl;
}

int main() {
    std::thread t1(&UpdateAndRead, &a, readvalues1);
    std::thread t2(&UpdateAndRead, &b, readvalues2);
    std::thread t3(&UpdateAndRead, &c, readvalues3);
    std::thread t4(&Read, readvalues4);
    std::thread t5(&Read, readvalues5);

    go.store(true); // default memory order so it will be visilbe to all threads at once

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    PrintArray(readvalues1);
    PrintArray(readvalues2);
    PrintArray(readvalues3);
    PrintArray(readvalues4);
    PrintArray(readvalues5);

    return 0;
}
