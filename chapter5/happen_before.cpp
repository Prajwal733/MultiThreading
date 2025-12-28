#include <iostream>

void Foo(int a,int b) {
    std::cout << a << " " << b << std::endl;
}

int GetNum() {
    static int i = 0;
    return ++i;
}

int main() {
    // output can be 2 1 or 1 2 (since they are not ordered)
    Foo(GetNum(), GetNum());

    std::cout << GetNum() << std::endl;

    std::cout << GetNum() << std::endl;

    return 0;
}
