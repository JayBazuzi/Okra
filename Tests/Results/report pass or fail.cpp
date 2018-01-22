#define OKRA_DO_NOT_DEFINE_TEST 1
#include "Okra.h"
using namespace okra;
using namespace okra::internals;

void PassingTestBody(bool &pass) { pass = true; }
TestInfo PassingTest{"/passing/test.cpp", "A passing test", PassingTestBody};

void FailingTestBody(bool &pass) { pass = false; }
TestInfo FailingTest{"/Failing/test.cpp", "A failing test", FailingTestBody};

void FailInTryCatchTestBody(bool &pass) { pass = false; }
TestInfo FailInTryCatchTest{"/FailInTryCatch/test.cpp", "A failing test", FailInTryCatchTestBody};

#define ASSERT_PASSED(operation) ASSERT_MESSAGE(operation, #operation " should pass");
#define ASSERT_FAILED(operation) ASSERT_MESSAGE(!operation, #operation " should fail");

OKRA_TEST("Multiple successful tests = pass")
{
	Tests subject;
	subject.Add(PassingTest);
	subject.Add(PassingTest);
	ASSERT_PASSED(subject.RunAll());
}

OKRA_TEST("Any test fails = fail")
{
	Tests subject;
	subject.Add(PassingTest);
	subject.Add(FailingTest);
	ASSERT_FAILED(subject.RunAll());
}

OKRA_TEST("No tests found is a failure")
{
	Tests subject;
	ASSERT_FAILED(subject.RunAll());
}

OKRA_TEST("Assertions work in try/catch") { ASSERT_FAILED(FailInTryCatchTest.Run({})); }

OKRA_TEST("Assertion failure ends the test")
{
	bool didTestContinue = false;
	const auto body = [&](bool &OKRA_pass) {
		ASSERT_EQUAL(42, 0);
		didTestContinue = true;
	};
	TestInfo{"", "", body}.Run({});
	ASSERT_EQUAL(false, didTestContinue);
}
