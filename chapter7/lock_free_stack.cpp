#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <optional>

using std::nullopt;

std::atomic<bool> start(false);

template<typename T>
class ThreadSafeStack {
public:
    ThreadSafeStack() {
        head_.store(nullptr);
    }

    void Push(T item) {
        Node* new_node = new Node();
        new_node->item = item;
        Node* old_head = head_.load();
        new_node->next = old_head;

        while (!head_.compare_exchange_weak(new_node->next, new_node, std::memory_order_seq_cst));
    }

    // not threadsafe
    void PrintStack() {
        Node* old_head = head_.load();

        while (old_head) {
            std::cout << old_head->item << " ";
            old_head = old_head->next;
        }

        std::cout << std::endl;
    }

    std::optional<T> Pop() {
        Node* old_head = head_.load();

        while ((old_head != nullptr) && (!head_.compare_exchange_weak(old_head, old_head->next, std::memory_order_seq_cst)));

        if (old_head == nullptr) {
            std::cout << "returning nullopt" << std::endl;
            return nullopt;
        }

        T item = old_head->item;
        // unsafe to delete this because some other thread may have access and can create dangling references.
        // good solution is to use std::atomic<shared_ptr<T>> for the next 
        delete old_head;
        std::cout << "returning an item " << item << std::endl;
        return item;
    }

private:
    struct Node {
        T item;
        Node* next;
    };

    std::atomic<Node*> head_;
};

void PushThread(ThreadSafeStack<int>* st) {
    while (!start.load());

    for (int i=0;i<10;i++) {
        st->Push(i);
        std::this_thread::yield();
    }
}

int main() {
    std::cout << std::thread::hardware_concurrency() << std::endl;
    std::vector<std::thread> threads;

    ThreadSafeStack<int>* st = new ThreadSafeStack<int>();
    for (int i=0;i<10;i++) {
        threads.push_back(std::thread(&PushThread, st));
    }

    start.store(true);

    for (std::thread& thread: threads) {
        thread.join();
    }

    st->PrintStack();

    int count = 0;
    while (st->Pop().has_value()) {
        count++;
    }

    std::cout << count << std::endl;

    return 0;
}
