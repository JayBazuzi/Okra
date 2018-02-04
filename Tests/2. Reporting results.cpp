#include "Okra.h"

TEST("When a test passes, its result is printed to the screen.")
{
	okra::TestInfo testInfo{"a test", []() {}};
	std::stringstream result;

	okra::internals::OStreamListener subject(result);
	subject.OnStart(testInfo);
	subject.OnEnd(testInfo, std::chrono::milliseconds(42));
	ASSERT_EQUAL("a test (42ms)\n", result.str());
}

TEST("When a test fails, its result is printed to the screen with a detail message.")
{
	std::stringstream result;
	okra::internals::OStreamListener subject(result);
	subject.OnFail("a detail message");
	ASSERT_EQUAL("a detail message", result.str());
}

TEST("prints the duration of the test on end")
{
	std::stringstream result;
	okra::internals::OStreamListener subject(result);
	subject.OnEnd({"a test", []() {}}, std::chrono::milliseconds(42));
	ASSERT_EQUAL(" (42ms)\n", result.str());
}
