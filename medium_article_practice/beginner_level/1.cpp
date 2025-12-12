#include <thread>
#include <iostream>

/*
we can create a thread in multiple ways
1. function 
2. object
3. functor 
4. lambda function 
*/

void DisplayMessage(std::string message) {
    std::cout << message << std::endl;
}

class Temp {
public:
   Temp() {
    // std::cout << "Creating a default object" << std::endl;
   }

  // copy constructor
  Temp(Temp& other) {
    std::cout << "You are trying to copy the object" << std::endl;
  }

  void DisplayMessage(std::string message) {
    std::cout << message << std::endl;
  }
};

class Func {
public:
    Func() {

    }

    Func(Func& other) {
        std::cout << "Trying to copy the functor " << std::endl;
    }

    void operator() (std::string message) {
        std::cout << message << std::endl;
    }
};

int main() {
    std::string message = "Hello from thread";

    std::thread fun_thread(&DisplayMessage, std::ref(message));

    Temp obj;
    std::thread class_thread(&Temp::DisplayMessage, &obj, std::ref(message));

    Func obj2;
    // If you don't pass as ref(obj2) it will first copy and then execute. 
    std::thread func_thread(std::ref(obj2), std::ref(message));

    std::thread lambda_thread([&message]{
        std::cout << message << std::endl;
    });

    fun_thread.join();
    class_thread.join();
    func_thread.join();
    lambda_thread.join();

    return 0;
}