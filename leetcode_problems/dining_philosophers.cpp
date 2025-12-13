class DiningPhilosophers {
public:
    DiningPhilosophers() {
    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        int first_lock = philosopher, second_lock = (first_lock + 1) % 5;
        
		std::scoped_lock lck(spoons_[first_lock], spoons_[second_lock]);
        pickLeftFork();
        pickRightFork();

        eat();

        putLeftFork();
        putRightFork();
    }

private:
    std::mutex spoons_[5];
};