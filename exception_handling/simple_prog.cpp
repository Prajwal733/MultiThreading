#include <bits/stdc++.h>
using namespace std;

class CustomException : public exception {
public:
    const char* what() const noexcept override {
        return "this is my exception";
    }
};

int main() {
    try {
        throw -1;
    } catch (int e) {
        cout << "Exception caught " << e << endl;
    }

    try {
        throw CustomException();
    } catch(CustomException &e) {
        cout << e.what() << endl;
    }

    return 0;
}
