#include "Okra.h"
#include <sstream>

TEST("prints the name of the test on start")
{
	std::stringstream result;
	okra::internals::OStreamListener subject(result);
	subject.OnStart({"foo.cpp", "a test", [](bool &) {}});
	ASSERT_EQUAL("a test", result.str());
}

TEST("prints the duration of the test on end")
{
	std::stringstream result;
	okra::internals::OStreamListener subject(result);
	subject.OnEnd({"foo.cpp", "a test", [](bool &) {}}, 42);
	ASSERT_EQUAL(" (0.042 ms)\n", result.str());
}