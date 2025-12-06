1. lock 

std::mutex 

 - std::lock_guard -> RAII for mutexes (std::adopt_lock, to prevent mtx.lock in constructor)
 - std::lock -> for locking multiple mutexes at once, 
 - std::scoped_lock -> combination of std::lock_guard & std::lock 

 - std::unique_lock -> can perform deferred lock, unlock (maintains a flag to track whether it owns the mutex or not) 

 - std::shared_lock & shared_mutex -> for read heavy systems (#include <shared_mutex>)
