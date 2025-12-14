#include <iostream>
#include <mutex>

class BankAccount {
public:
    BankAccount() {
        balance_ = 0;
    }

    BankAccount(int default_balance) : balance_(default_balance) {
    }

    void Deposit(int money) {
        if (money <= 0) {
            std::cout << "Money is invalid" << std::endl;
            return;
        }

        std::lock_guard lck(balance_lock_);
        balance_ += money;
    }

    void WithDraw(int money) {
        // std::cout can take some time so we will use this to track the op
        bool success = true;
        {
            std::lock_guard lck(balance_lock_);
            if (balance_ >= money) {
                balance_ -= money;
            } else {
                success = false;
            }
        }

        if (!success) {
            std::cout << "Insufficient balance" << std::endl;
        }
    }

    int GetBalance() {
        std::lock_guard lck(balance_lock_);
        return balance_;
    }

private:
    int balance_;
    std::mutex balance_lock_;
};

int main() {
    BankAccount acc(25);

    acc.Deposit(10);
    acc.WithDraw(35);

    acc.WithDraw(61);

    std::cout << acc.GetBalance() << std::endl;

    return 0;
}
