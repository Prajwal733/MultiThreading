#include <iostream>
#include <atomic>

int main() {
    std::atomic<int> x;
    // To check whether a particular atomic<T> is lock free implemented or not. 
    std::cout << x.is_lock_free() << std::endl;


    // To check whether a particular type is lock free or not
    // output -> (0, 1, 2) -> 0 (never lock free)  1 (depends on the hardware) 2 (always lock free)
    std::cout << ATOMIC_BOOL_LOCK_FREE << std::endl;

    std::cout << ATOMIC_INT_LOCK_FREE << std::endl;

    std::cout << ATOMIC_LONG_LOCK_FREE << std::endl;

    std::cout << ATOMIC_POINTER_LOCK_FREE << std::endl;

    // std::atomic_flag (always lock free) supports only 2 methods: clear() test_and_set() 
    std::atomic_flag af(1); // can be initiazed to 0 or 1

    std::cout << af.test_and_set() << std::endl; // 0 
    std::cout << af.test_and_set() << std::endl; // 1
    af.clear();
    std::cout << af.test_and_set() << std::endl; // 0 

    

    return 0;
}