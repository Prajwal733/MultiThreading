#include <bits/stdc++.h>
using namespace std;

class Network {
public:
    int GetSize(string url) {
        return get_func_(url);
    }

    vector<char> fetch_bytes(string url, int start, int end) {
        vector<char> bytes(end - start + 1, 'a');
        return bytes;
    }

    void SetGetSize(function<int(string url)> get_func) {
        get_func_ = get_func;
    }

private:
    function<int(string url)> get_func_;
};

class MultiDownloader {
public:
    MultiDownloader(string url) : url_(url) {
    }

    void DownloadFromNetwork(Network n) {
        int total_size = n.GetSize(url_);
        cout << "Total size " << total_size << endl;
        data_.resize(total_size);
        cout << data_.size() << endl;

        int max_threads = std::thread::hardware_concurrency();
        if (max_threads == 0) {
            cout << "Failed to read the hardware concurrency, continuing with single thread" << endl;
            max_threads = 1;
        }

        if (max_threads > total_size) {
            max_threads = total_size;
        }

        int each_thread_data;
        if (max_threads == 1) {
            each_thread_data = total_size;
        } else {
            // last thread will get the remainder
            each_thread_data = (total_size / (max_threads - 1));
        }

        cout << each_thread_data << " " << max_threads << endl;

        vector<thread> threads;

        for(int i=0;i<max_threads;i++) {
            threads.push_back(thread([this, i, &each_thread_data, &total_size, &n] {
                int start_byte = i * each_thread_data, end_byte = min(start_byte + each_thread_data - 1, total_size -1);
                vector<char> bytes = n.fetch_bytes(url_, start_byte, end_byte);
                cout << "start and end byte " << start_byte << " " << end_byte << endl;

                std::lock_guard lck(data_mutex_);
                for(int k=0;k<bytes.size();k++) {
                    data_[k + start_byte] = bytes[k];
                }
            }));
        }

        for(thread& thread: threads) {
            thread.join();
        }
    }

    vector<char> GetData() const {
        // Here we don't need lock, our goal is not to make DownloadFromNetwork class
        // thread safe, but only to achieve multithreading in Download method, since
        // Download method is blocking call, this is simply fine. 
        return data_;
    }

private:
   string url_;
   mutex data_mutex_;
   vector<char> data_;
};

int main() {
    Network n;
    function<int(string)> f = [](string url) {
        return 100;
    };
    n.SetGetSize(f);

    MultiDownloader m("google.com");
    m.DownloadFromNetwork(n);
    vector<char> bytes = m.GetData();

    for(auto x: bytes){
        cout << x << " ";
    }

    cout << endl;

    return 0;
}
