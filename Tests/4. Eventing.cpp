#include "Okra.h"
#include <sstream>

class StringListener : public okra::IListener
{
public:
	std::vector<okra::TestInfo> starts;
	void OnStart(const okra::TestInfo &testInfo) override { starts.push_back(testInfo); }

	std::vector<okra::TestInfo> ends;
	void OnEnd(const okra::TestInfo &testInfo, std::chrono::high_resolution_clock::duration duration) override { ends.push_back(testInfo); }

	void OnFail(const std::string &message) override {}
};

TEST("when running a test")
{
	auto string_listener = std::make_shared<StringListener>();

	auto subject = okra::TestInfo({"A test", []() {}});
	subject.Run({string_listener});

	ASSERT_EQUAL(1, string_listener->starts.size());
	ASSERT_EQUAL(subject.name, string_listener->starts[0].name);
	ASSERT_EQUAL(1, string_listener->ends.size());
	ASSERT_EQUAL(subject.name, string_listener->ends[0].name);
}

TEST("prints the name of the test on start")
{
	std::stringstream result;
	okra::internals::OStreamListener subject(result);
	subject.OnStart({"a test", []() {}});
	ASSERT_EQUAL("a test", result.str());
}
