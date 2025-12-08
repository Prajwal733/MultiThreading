class LFUCache {
public:
    LFUCache(int capacity) {
        cout << "constructor " << endl;
        capacity_ = capacity;
        current_time_ = 0;
    }
    
    int get(int key) {
        cout << "Get " << key << endl;
        auto itr = data_.find(key);
        if (itr == data_.end()) {
            return -1;
        }

        int value = data_[key];
        UpdateAccessForKey(key);
        return value;
    }
    
    void put(int key, int value) {
        cout << "Put " << key << " " << value << endl;
        if (data_.find(key) != data_.end()) {
            data_[key] = value;
        } else {
            if (data_.size() == capacity_) {
                Evict();
            }

            data_[key] = value;
            access_[key] = {0, current_time_};
            cout << "updated the data store " << endl;
        }

        UpdateAccessForKey(key);
    }

private:
    void Evict() {
        cout << "Printing" << endl;
        for(auto x: order_) {
            cout << x.first.first << " " << x.first.second << " " << x.second << endl;
        }
        cout << "the end" << endl;

        
        auto itr = order_.begin();
        cout << "Evicted " << itr->second << endl;
        data_.erase(itr->second);
        access_.erase(itr->second);
        order_.erase(order_.begin());
    }

    void UpdateAccessForKey(int key) {
        current_time_++;

        /* access_, order_ */
        int freq = access_[key].first, time = access_[key].second;
        auto itr = order_.find({{freq, time}, key});

        if (itr != order_.end())
             order_.erase(itr);
    
        cout << "Removed from the order" << endl;
        access_[key] = {freq + 1, current_time_};
        order_.insert({access_[key], key});
    }
    std::map<int, int> data_;
    std::map<int, pair<int,int>> access_;
    set<pair<pair<int,int>, int>> order_;
    int capacity_;
    int32_t current_time_;
};