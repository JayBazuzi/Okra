#include "Okra.h"
#include "_TestHelpers/ArtificialClock.h"

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



ArtificialClock::time_point ArtificialClock::nowMicroseconds;

TEST("The duration of the test is reported")
{
	const ArtificialClock::duration result = okra::internals::duration_to_execute<ArtificialClock>(
	    []() { ArtificialClock::advance(ArtificialClock::duration(42)); });
	ASSERT_EQUAL(42, std::chrono::duration_cast<std::chrono::microseconds>(result).count());
}
