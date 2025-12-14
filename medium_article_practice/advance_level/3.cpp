#include <bits/stdc++.h>
using namespace std;

class Network {
public:
    Network(function<string(string)> get_content_f) {
        get_content_f_ = get_content_f;
    }

    string GetContent(string url) {
        return get_content_f_(url);
    }

private:
    function<string(string)> get_content_f_;
};

class WebCrawler {
public:
    WebCrawler(int available_threads, Network& n) : available_threads_(available_threads), network_(n)  {
        num_wait_threads_ = 0;
    }

    set<string> CrawlFrom(vector<string> start_urls) {
        for(string url: start_urls) {
            known_urls_.insert(url);
            process_urls_.push(url);
        }

        vector<thread> threads;

        for(int i=0;i<available_threads_;i++) {
            threads.push_back(thread(&WebCrawler::ThreadWork, this));
        }

        for(thread& thread: threads) {
            thread.join();
        }

        return known_urls_;
    }

private:
    void ThreadWork() {
        while (true) {
            unique_lock lck(url_mutex_);

            num_wait_threads_++;
            // cout << num_wait_threads_ << endl;
            url_signal_.wait(lck, [&] {
                return process_urls_.size() != 0 || num_wait_threads_ == available_threads_;
            });

            if (process_urls_.size() == 0) {
                url_signal_.notify_all();
                return;
            }

            num_wait_threads_--;

            string url_to_process = process_urls_.front();
            process_urls_.pop();
            lck.unlock();

            string content = network_.GetContent(url_to_process);
            vector<string> urls = GetUrlsFromContent(content);

            lock_guard lck1(url_mutex_);
            for(string url: urls) {
                if (known_urls_.find(url) == known_urls_.end()) {
                    known_urls_.insert(url);
                    process_urls_.push(url);
                }
            }
            url_signal_.notify_all();
        }
    }

    vector<string> GetUrlsFromContent(string content) {
        // assume content are divided by spaces
        // cout << content << endl;
        vector<string> urls;
        string url = "";
        for(int i=0;i<content.size();i++) {
            if (content[i] == ' ') {
                urls.push_back(url);
                url = "";
            } else {
                url += content[i];
            }
        }

        if (url.size() != 0) {
            urls.push_back(url);
        }

        return urls;
    }

    mutex url_mutex_;
    condition_variable url_signal_;
    set<string> known_urls_;
    int num_wait_threads_;
    Network network_;
    int available_threads_;
    queue<string> process_urls_;
};

int main() {
    map<string, string> m = {
        {"google.com", "facebook.com pinterest.com"}, 
        {"facebook.com", "gemini.com another.com"},
        {"pinterest.com", "gemini.com"},
        {"gemini.com", ""},
        {"another.com", ""}, 
    };

    function<string(string)> f = [&m](string url){
        // cout << "Network call for " << url << endl;
        return m[url];
    };

    Network n(f);

    WebCrawler wc(4, n);

    set<string> urls = wc.CrawlFrom({"google.com"});

    for(auto x: urls) {
        cout << x << endl;
    }

    return 0;
}
