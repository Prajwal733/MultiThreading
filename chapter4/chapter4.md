1. Condition Variables -> cv.wait(), cv.notify_one(), cv.notify_all()

2. Futures -> std::async(func, args...)
    std::launch::async
    std::launch::deferred 

3. Promises -> std::promise<int> f; 
    f.get_future()

    f.set_value(4)

4. package_task -> std::package_task<int(int, string)> pt(func)
    pt.get_future()

    get()

    useful than function<> because easy to capture the return value through future

5. latches
   std::latch ltc(X)

   ltc.wait()

   ltc.count_down()

   ltc.is_ready()

6. barries 
   std::barrier b(x)

   b.arrive_and_wait()
   b.arrive_and_drop()
