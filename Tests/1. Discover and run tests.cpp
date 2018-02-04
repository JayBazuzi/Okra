#include "Okra.h"

TEST("It runs all the tests.")
{
	okra::internals::Tests subject;

	bool test1Ran = false;
	subject.Add(okra::TestInfo{"test 1", [&] { test1Ran = true; }});

	bool test2Ran = false;
	subject.Add(okra::TestInfo{"test 2", [&] { test2Ran = true; }});

	subject.RunAll({});
	ASSERT(test1Ran && test2Ran);
}

TEST("If all tests pass, the run passes.")
{
	okra::internals::Tests subject;
	subject.Add(okra::TestInfo{"test 1", [&] {}});
	subject.Add(okra::TestInfo{"test 2", [&] {}});
	ASSERT(subject.RunAll({}));
}

TEST("If any tests fail, the run fails.")
{
	okra::internals::Tests subject;
	subject.Add(okra::TestInfo{"test 1", [&] { FAIL(""); }});
	subject.Add(okra::TestInfo{"test 2", [&] {}});
	ASSERT(!subject.RunAll({}));
}

TEST("If no tests run, the run fails.")
{
	okra::internals::Tests subject;
	ASSERT(!subject.RunAll({}));
}

class ArtificialClock
{
public:
	using duration = std::chrono::duration<uint64_t, std::ratio<1, 1000000>>;
	using time_point = std::chrono::time_point<ArtificialClock>;

	static void advance(duration d) noexcept { nowMicroseconds += d; }
	static time_point now() noexcept { return nowMicroseconds; }

	static const bool is_steady = false;

private:
	static time_point nowMicroseconds;
};

ArtificialClock::time_point ArtificialClock::nowMicroseconds;

TEST("the body of a test that takes < 1 ms time is timed correctly")
{
	const auto result = okra::internals::time_to_execute_microseconds<ArtificialClock>(
	    []() { ArtificialClock::advance(std::chrono::microseconds(100)); });
	ASSERT_EQUAL(100, result);
}

TEST("the body of a test that takes > 1 ms time is timed correctly")
{
	const auto result = okra::internals::time_to_execute_microseconds<ArtificialClock>(
	    []() { ArtificialClock::advance(std::chrono::microseconds(1009)); });
	ASSERT_EQUAL(1009, result);
}
