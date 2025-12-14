#include <bits/stdc++.h>
using namespace std;

template<typename T>
class BoundedQueue {
public:
    BoundedQueue(int capacity) : capacity_(capacity) {
    }

    void Enqueue(T item) {
        unique_lock lck(items_mutex_);

        items_signal_.wait(lck, [&] {
            return items_.size() < capacity_;
        });

        items_.push(item);
        // Here we need to notify all because both types of threads
        // can be waiting for the signal (enqueue and Dequeue)
        items_signal_.notify_all();
    }

    T Dequeue() {
        unique_lock lck(items_mutex_);

        items_signal_.wait(lck, [&] {
            return items_.size() != 0;
        });

        // Here we are copying T, but if T is large enough either
        // we can store T* or in the Dequeue method we can accept 
        // reference of T& to store, but to mimic the standard queue
        // going with this. 
        T item = items_.front();
        items_.pop();
        items_signal_.notify_all();
        return item;
    }

    size_t Size() {
        unique_lock lck(items_mutex_);
        return items_.size();
    }

private:
    int capacity_;
    mutex items_mutex_;
    condition_variable items_signal_;
    queue<T> items_;
};

int main() {
    BoundedQueue<int> q(5);
    q.Enqueue(1);

    int f  = q.Dequeue();

    cout << f << " " << q.Size() << endl;

    return 0;
}
