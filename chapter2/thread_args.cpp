#include <iostream>
#include <thread>

class Data {
public:
  // Constructor 
  Data() {
    private_data_ = 0;
  }

  // Copy Constructor
  Data(Data& other) {
    std::cout << "Copy Constructor is called" << std::endl;
  }

  // Copy assignment
  Data& operator= (Data& other) {
    std::cout << "Copy assignment is called" << std::endl;
    return *this;
  }

  // Move constructor
  Data(Data&& other) {
    std::cout << "Move constructor is called" << std::endl;
  }

  // Move Assignment
  Data& operator= (Data&& other) {
    std::cout << "Move assignment is called" << std::endl;
    return *this;
  }

  int GetData() {
    return private_data_;
  }

private:
  int private_data_;
};

void MyWork(Data& d) {
    std::cout << "Doing my work " << std::endl;
}

int main() {
    Data d;

    // To pass d as reference, copy constructor will not be called here
    std::thread t1(MyWork, std::ref(d));

    Data d2 = d; // copy constructor
    d = d2; // copy assignment
    Data d3 = std::move(d); // move Constructor

    d = std::move(d3); // move assignment
    t1.join();

    // unique pointer experiment
    std::unique_ptr<Data> ud1 = std::make_unique<Data>();
    std::unique_ptr<Data> ud2 = std::move(ud1);

    if(ud1 == nullptr) {
        std::cout << "D1 is null after transferring the ownership" << std::endl;
    }

    return 0;
}