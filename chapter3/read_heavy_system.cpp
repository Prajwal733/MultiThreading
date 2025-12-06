#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include <shared_mutex>

using std::string;

class DNS {
public:
    void AddEntry(string key, string value) {
        // exclusive lock
        std::lock_guard lck(mtx);
        data_store_[key] = value;
    }

    string GetValue(string key) {
        // shared lock, many threads can read concurrently 
        std::shared_lock lck(mtx);
        auto itr = data_store_.find(key);
        if (itr == data_store_.end())
            return "";
        
        return itr->second;
    }
private:
  std::shared_mutex mtx;
  std::map<string, string> data_store_;
};

int main() {
    DNS d;

    d.AddEntry("a", "b");

    std::cout << d.GetValue("a") << std::endl;

    return 0;
}