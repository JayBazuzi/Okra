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

	static void advance(duration duration) noexcept { nowMicroseconds += duration; }
	static time_point now() noexcept { return nowMicroseconds; }

private:
	static time_point nowMicroseconds;
};

ArtificialClock::time_point ArtificialClock::nowMicroseconds;

TEST("the body of a test that takes < 1 ms time is timed correctly")
{
	const ArtificialClock::duration result = okra::internals::duration_to_execute<ArtificialClock>(
	    []() { ArtificialClock::advance(std::chrono::microseconds(2)); });
	ASSERT_EQUAL(2000, result.count());
}

TEST("the body of a test that takes > 1 ms time is timed correctly")
{
	const ArtificialClock::duration result = okra::internals::duration_to_execute<ArtificialClock>(
	    []() { ArtificialClock::advance(std::chrono::microseconds(42)); });
	ASSERT_EQUAL(42, std::chrono::duration_cast<std::chrono::microseconds>(result).count());
}
