#include <iostream>
#include <atomic>

int main() {
    int arr[5] = {1, 2, 3, 4, 5};

    std::atomic<int*> a_ptr(arr);

    auto ptr1 = a_ptr.load();
    std::cout << *ptr1 << std::endl; // 1

    ptr1 = a_ptr.fetch_add(3);
    std::cout << *ptr1 << std::endl; // 1 because it returns the previously stored value

    ptr1 = a_ptr.load();
    std::cout << *ptr1 << std::endl; // 4, pointer arithmetic 

    /*
    is not a single atomic read-modify-write operation. It expands conceptually to:
        1. Atomic load of a
        2. Non-atomic computation: value >> 1
        3. Atomic store back to a
    */
    std::atomic<int> a(5);
    a = a >> 1;

    std::atomic<int> b(15);

    std::cout << a << " " << b << std::endl;

    std::cout << (b/a) << std::endl;

    return 0;
}
