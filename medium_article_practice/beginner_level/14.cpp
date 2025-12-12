#include <iostream>
#include <thread>
#include <vector>
#include <future>

// defined int as long long to accomodate sum in int type :(
#define int long long 

int GetSum(int start, int end) {
    int sum = 0;

    for(int i=start;i<=end;i++) {
        sum += i;
    }

    return sum;
}

int GetParallelSum(int start, int end, int num_threads) {
    if (num_threads == 1) {
        return GetSum(start, end);
    }

    std::vector<std::future<int>> futures;

    int dis = (end - start + 1);
    int each = dis / (num_threads-1);

    while (start <= end) {
        std::future<int> f1 = std::async(&GetSum, start, std::min(start + each, end));
        start += (each + 1);
        futures.push_back(std::move(f1));
    }

    int sum = 0;
    for(std::future<int>& future: futures) {
        sum += future.get();
    }

    return sum;
}

int32_t main() {
    int max_threads = std::thread::hardware_concurrency();

    std::cout << max_threads << std::endl;

    std::cout << "Operations    |   Single Threaded    | MultiThreaded  |   IsSumEqual  " << std::endl;
    for(int i=10;i<=1000000000;i = i * 10) {
        auto t1 = std::chrono::steady_clock::now();
        int sum1 = GetParallelSum(1, i, 1);
        auto t2 = std::chrono::steady_clock::now();

        auto t3 = std::chrono::steady_clock::now();
        int sum2 = GetParallelSum(1, i, max_threads - 2);
        auto t4 = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> elapsed_ms1 = t2 - t1;
        std::chrono::duration<double, std::milli> elapsed_ms2 = t4 - t2;

        std::cout << i << " " << elapsed_ms1.count() << " " << elapsed_ms2.count() << " " << (sum1==sum2) << std::endl;
    }

    return 0;
}

/*
output:

12
Operations    |   Single Threaded    | MultiThreaded  |   IsSumEqual  
10 0.000191 0.867702 1
100 0.00018 1.14894 1
1000 0.001022 0.716849 1
10000 0.009548 1.2595 1
100000 0.100401 1.60026 1
1000000 1.01765 2.06425 1
10000000 8.79144 2.46617 1
100000000 82.343 17.1816 1
1000000000 824 159.268 1

*/