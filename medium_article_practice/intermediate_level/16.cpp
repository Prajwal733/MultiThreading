#include <bits/stdc++.h>
using namespace std;

/*
    This code can lead to deadlock, the fix would be to keep an eye on active therads
    in the ThreadPool
*/

class ThreadPoolForMergeSort {

struct Parameters {
    vector<int>& v;
    int i;
    int j;
    ThreadPoolForMergeSort& obj;
};

public:
    ThreadPoolForMergeSort(int num_threads) : stop_signal_(false), num_threads_(num_threads) {
        for(int i=0;i<num_threads_;i++) {
            threads_.push_back(thread(&ThreadPoolForMergeSort::ThreadWork, this));
        }
    }

    future<void> AddTask(function<void(vector<int>&, int ,int, ThreadPoolForMergeSort&)> f, vector<int>& v, int i, int j, ThreadPoolForMergeSort& obj) {
        packaged_task task(f);
        Parameters p = {v, i , j , obj};
        future<void> return_future = task.get_future();

        {
            lock_guard lck(task_mutex_);
            tasks_.push({std::move(task), p});
            task_notify_.notify_one();
        }

        return return_future;
    }

    ~ThreadPoolForMergeSort() {
        {
            lock_guard lck(task_mutex_);
            stop_signal_ = true;
            task_notify_.notify_all();
        }

        for(thread& thread: threads_) {
            thread.join();
        }
    }

private:
    void ThreadWork() {
        while (true) {
            unique_lock lck(task_mutex_);

            task_notify_.wait(lck, [&] {
                return tasks_.size() != 0 || stop_signal_;
            });

            if (tasks_.size() == 0) {
                task_notify_.notify_all();
                return;
            }

            packaged_task<void(vector<int>&, int, int, ThreadPoolForMergeSort&)> task = move(tasks_.front().first);
            Parameters p = tasks_.front().second;
            tasks_.pop();
            lck.unlock();
            
            task(p.v, p.i, p.j, p.obj);
        }
    }

    mutex task_mutex_;
    condition_variable task_notify_;
    bool stop_signal_;
    int num_threads_;
    vector<thread> threads_;

    queue<pair<packaged_task<void(vector<int>&, int ,int, ThreadPoolForMergeSort&)>, Parameters>> tasks_;
};

void MergeSort(vector<int>& v, int i, int j, ThreadPoolForMergeSort& tp) {
    cout << "Merge sort called with " << i << " " << j << endl;
    if (i >= j) {
        return;
    }

    int mid = (j - i) / 2  + i;
    
    if (v.size() >= 1000) {
        // AddTask can lead to deadlock if the number of task becomes high
        future<void> f = tp.AddTask(MergeSort, v, i, mid, tp);
        f.wait();
    } else {
        MergeSort(v, i, mid, tp);
    }

    MergeSort(v, mid+1, j, tp);
    int len = j - i + 1, left = i, right = mid + 1;
    vector<int> temp;

    while((left <= mid) && (right <= j)) {
        if (v[left] < v[right]) {
            temp.push_back(v[left]);
            left++;
        } else {
            temp.push_back(v[right]);
            right++;
        }
    }

    while(left <= mid) {
        temp.push_back(v[left]);
        left++;
    }

    while(right <= j) {
        temp.push_back(v[right]);
        right++;
    }

    for(int k = 0;k<temp.size();k++) {
        v[k + i] = temp[k];
    }
}

int main() {
    ThreadPoolForMergeSort tp(8);
    vector<int> v = {9, 3, 2, 7, 29, 1, 20};

    MergeSort(v, 0, v.size() - 1, tp);

    for(int& x: v) {
        cout << x << endl;
    }

    return 0;
}
