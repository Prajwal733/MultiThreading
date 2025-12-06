#include <iostream>
#include <mutex>
#include <thread>
#include <functional>

class Data {
private:
    int a_;

public:
    Data() {
        a_ = 0;
    }

    void SetData(int x) {
        a_ = x;
    }

    int GetData() {
        return a_;
    }
};

class DataProtector {
private:
 Data d_;
 std::mutex d_mutex_;

public:
    void PerformOperation(std::function<void(Data&)> func) {
        std::lock_guard lck(d_mutex_);
        std::cout << "Value of data after aquiring lock " << d_.GetData() << std::endl;
        func(d_);

        std::cout << "Value of data just before releasing lock " << d_.GetData() << std::endl;
    }
};

Data* wrongly_useful = nullptr;
void DoesWrongThing(Data& protect) {
    wrongly_useful = &protect;
    wrongly_useful->SetData(-1);
}

int main() {
  DataProtector d;
  std::thread child_thread(&DataProtector::PerformOperation, &d, DoesWrongThing);

  for(int i=0;i<1000000;i++) {
    if (wrongly_useful)
       wrongly_useful->SetData(i);
  }

  child_thread.join();

  return 0;
}