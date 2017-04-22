#include "Okra.h"
#include <chrono>

using namespace okra;
using namespace okra::internals;

class Clock {
    public:
        using rep = uint64_t;
        using period = std::ratio<1, 1000000>;
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<Clock>;

        static void advance(duration d) noexcept {
            nowMicroseconds += d;
        }
        static time_point now() noexcept {
            return nowMicroseconds;
        }

        static const bool is_steady;

    private:
        static time_point nowMicroseconds;
};

Clock::time_point Clock::nowMicroseconds;
const bool Clock::is_steady = false;

Example("the body of a test that takes no time is timed correctly") {
    AssertEqual(time_to_execute_microseconds<Clock>([](){}), 0);
}

Example("the body of a test that takes < 1 ms time is timed correctly") {
    AssertEqual(time_to_execute_microseconds<Clock>([](){Clock::advance(std::chrono::microseconds(100));}), 100);
}

Example("the body of a test that takes > 1 ms time is timed correctly") {
    AssertEqual(time_to_execute_microseconds<Clock>([](){Clock::advance(std::chrono::microseconds(1009));}), 1009);
}

