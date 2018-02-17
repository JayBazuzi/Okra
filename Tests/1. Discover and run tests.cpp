#include "Okra.h"

TEST("It runs all the tests.")
{
	okra::internals::Tests subject;

	bool test1Ran = false;
	subject.Add(okra::TestInfo{"file.cpp", "test 1", [&] { test1Ran = true; }});

	bool test2Ran = false;
	subject.Add(okra::TestInfo{"file.cpp", "test 2", [&] { test2Ran = true; }});

	subject.RunAll({});
	ASSERT(test1Ran && test2Ran);
}

TEST("If all tests pass, the run passes.")
{
	okra::internals::Tests subject;
	subject.Add(okra::TestInfo{"file.cpp", "test 1", [&] {}});
	subject.Add(okra::TestInfo{"file.cpp", "test 2", [&] {}});
	ASSERT(subject.RunAll({}));
}

TEST("If any tests fail, the run fails.")
{
	okra::internals::Tests subject;
	subject.Add(okra::TestInfo{"file.cpp", "test 1", [&] { FAIL(""); }});
	subject.Add(okra::TestInfo{"file.cpp", "test 2", [&] {}});
	ASSERT(!subject.RunAll({}));
}

TEST("If no tests run, the run fails.")
{
	okra::internals::Tests subject;
	ASSERT(!subject.RunAll({}));
}
