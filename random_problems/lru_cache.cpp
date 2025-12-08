#include <iostream>
#include <map>
#include <memory>

/*
    map -> key , Node*

    linked list -> (key, value, next, previous)

       -> Get value at Node*  
       -> insert and return Node * 
       -> make space 
*/

template<typename Key, typename Value>
struct Node {
        Node(Key key, Value value) {
            this->key = key;
            this->value = value;
            previous = nullptr;
            next = nullptr;
        }

        Key key;
        Value value;
        Node* previous;
        Node* next;
};

template<typename Key, typename Value>
class DoublyLinkedList {
typedef struct Node<Key, Value> Node;
public:
    DoublyLinkedList() {
        head_  = nullptr;
        tail_ = nullptr;
    }

    Node* Insert(Key key, Value value) {
        Node * current_node = new Node(key, value);
        InsertAtHead(current_node);

        return current_node;
    }

    void UpdateGetValue(Node* node) {
        // left part 
        if (node->previous) {
            node->previous->next = node->next;
        }

        // right part 
        if (node->next) {
            node->next->previous = node->previous;
        }

        if (head_ == node) {
            head_ = head_->next;
        }

        if (tail_ == node) {
            tail_ = tail_->previous;
        }

        node->next = nullptr;
        node->previous = nullptr;

        InsertAtHead(node);
    }

    int MakeSpace() {
        int remove_key = tail_->key;

        if (head_ == tail_) {
            delete tail_;
            tail_ = nullptr;
            head_ = nullptr;
        } else {
            Node * temp_tail = tail_->previous;
            delete tail_;
            tail_ = temp_tail;
            tail_->next = nullptr;
        }

        return remove_key;
    }

private:
    void InsertAtHead(Node* current_node) {
        if (head_) {
            head_->previous = current_node;
            current_node->next = head_;
            head_ = current_node;
        } else {
            head_ = current_node;
            tail_ = current_node;
        }
    }
    Node * head_;
    Node * tail_;
};

class LRUCache {
public:
    LRUCache(int capacity) {
        capacity_ = capacity;
        linked_list_ = std::make_unique<DoublyLinkedList<int,int>>();
    }
    
    int get(int key) {
        auto itr = store_.find(key);

        if (itr == store_.end()) {
            return -1;
        }

        int value = itr->second->value;
        linked_list_->UpdateGetValue(itr->second);

        return value;
    }
    
    void put(int key, int value) {
        auto itr = store_.find(key);

        if (itr != store_.end()) {
            itr->second->value = value;
            linked_list_->UpdateGetValue(itr->second);
            return;
        }

        if (store_.size() == capacity_) {
            int removed_key = linked_list_->MakeSpace();
            store_.erase(removed_key);
        }

        Node<int,int>* entry = linked_list_->Insert(key, value);
        store_[key] = entry;
    }

private:
    int capacity_;
    std::map<int, Node<int,int>*> store_;
    std::unique_ptr<DoublyLinkedList<int,int>> linked_list_;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

 int main() {
    int capacity = 2;
    LRUCache* obj = new LRUCache(capacity);
    int key = 2;
    int param_1 = obj->get(key);
    std::cout << param_1 << std::endl;
    int value = 5;
    obj->put(key,value);
    param_1 = obj->get(key);
    std::cout << param_1 << std::endl;
    obj->put(8, 2);
    param_1 = obj->get(key);
    std::cout << param_1 << std::endl;
    param_1 = obj->get(8);
    std::cout << param_1 << std::endl;

    obj->put(10, 6);
    std::cout << obj->get(2) << std::endl;
    std::cout << obj->get(8) << std::endl;
    std::cout << obj->get(10) << std::endl;

    return 0;
 }