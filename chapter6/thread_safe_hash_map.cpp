#include <iostream>
#include <mutex>
#include <vector>
#include <optional>
#include <shared_mutex>
#include <memory>
#include <functional> // for std::hash<T> 

// found later, lazy to update everywhere
using std::unique_ptr;

template<typename Key, typename Value>
class Bucket {
typedef std::pair<Key, Value> bucket_value;
typedef std::vector<bucket_value> bucket;
public:
    // write lock
    void AddEntry(Key key, Value value) {
        std::lock_guard lck(entry_mutex_);
        auto found = FindEntry(key);
        
        if (found == entries_.end()) {
            entries_.push_back({key, value});
        } else {
            std::cout << "Trying to insert duplicate key" << std::endl;
        }
    }

    // write lock
    void UpdateEntry(Key key, Value value) {
        std::lock_guard lck(entry_mutex_);
         auto found = FindEntry(key);
         if (found == entries_.end()) {
            entries_.push_back({key, value});
         } else {
            found->second = value;
         }
    }

    // write lock
    void DeleteEntry(Key key) {
        std::lock_guard lck(entry_mutex_);
        auto found = FindEntry(key);
        if (found != entries_.end()) {
           entries_.erase(found);
        } else {
            std::cout << "Key not found" << std::endl;
        }
    }

    // read lock
    std::optional<Value> GetValue(Key key) {
        std::shared_lock lck(entry_mutex_);
        auto found = FindEntry(key);
        if(found != entries_.end()) {
            return found->second;
        }

        return std::nullopt;
    }

private:
  typename bucket::iterator FindEntry(Key key) {
    auto it = entries_.begin();
    while(it != entries_.end()) {
        if (it->first == key) {
            return it;
        }
        it++;
    }
    return it;
  }

  bucket entries_;
  std::shared_mutex entry_mutex_;
};

template<typename Key, typename Value>
class HashMap {
public:
  HashMap(int num_buckets = 19) {
    std::cout << "Intializing the hashmap " << num_buckets << std::endl;
    num_buckets_ = num_buckets;
    buckets_.resize(num_buckets);

    for(int i=0;i<num_buckets_;i++) {
        buckets_[i] = std::make_unique<Bucket<Key, Value>>();
    }
  }

   void AddEntry(Key key, Value value) {
     buckets_[GetBucket(key)]->AddEntry(key, value);
   }

   void UpdateEntry(Key key, Value value) {
     buckets_[GetBucket(key)]->UpdateEntry(key, value);
   }

   void DeleteEntry(Key key) {
    buckets_[GetBucket(key)]->DeleteEntry(key);
   }

   std::optional<Value> GetValue(Key key) {
    return buckets_[GetBucket(key)]->GetValue(key);
   }

private:
    std::vector<unique_ptr<Bucket<Key, Value>>> buckets_;
    int num_buckets_;
    std::hash<Key> hasher;

    int GetBucket(Key key) {
        int bucket = hasher(key) % num_buckets_;
        // std::cout << bucket << std::endl;
        return bucket;
    }
};

int main() {
    HashMap<int, int> b(5);

    b.AddEntry(1, 2);
    b.AddEntry(5, 6);
    b.AddEntry(5, 4);
    b.UpdateEntry(1, 3);
    std::optional<int> ans = b.GetValue(1);
    if (ans.has_value()) {
        std::cout << ans.value() << std::endl;
    }


    b.DeleteEntry(1);
    std::optional<int> ans1 = b.GetValue(1);

    if(!ans1.has_value()) {
        std::cout << "Everything working fine " << std::endl;
    }

    return 0;
}