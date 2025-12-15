#include <iostream>
#include <atomic>
#include <vector>
using namespace std;

struct alignas(64) Seat {
    std::atomic_flag isBooked = ATOMIC_FLAG_INIT;
};

class TicketBookingSystem {
public:
    TicketBookingSystem(int seats) {
        num_available_seats_ = seats;

        for(int i=0;i<seats;i++) {
            seats_.push_back(new Seat());
        }
    }

    bool BookSeat(int seat_index) {
        if ((seat_index < 0) or (seat_index >= seats_.size())) {
            cout << "Invalid seat index" << endl;
            return false;
        }

        bool state = seats_[seat_index]->isBooked.test_and_set();

        if (state) {
            cout << "Seat is already booked" << endl;
            return false;
        }

        // First decrementing the available seats would be better becasue 
        // cout can take some time. 
        num_available_seats_--;
        cout << "Booking successfull" << endl;
        return true;
    }

    int GetAvailableSeats() const {
        return num_available_seats_;
    }

    // Clean up, avoid memory leak
    ~TicketBookingSystem() {
        for (auto x: seats_) {
            delete x;
        }
    }

private:
    vector<Seat*> seats_;
    atomic<int> num_available_seats_;
};

int main() {
    TicketBookingSystem t(5);

    bool ans1 = t.BookSeat(2);
    bool ans2 = t.BookSeat(2);

    int ans3 = t.GetAvailableSeats();

    cout << ans1 << " " << ans2 << " " << ans3 << endl;

    return 0;
}
