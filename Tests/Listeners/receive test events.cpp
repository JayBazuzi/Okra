#include "Okra.h"
#include <sstream>

class StringListener : public okra::IListener
{
public:
	std::vector<okra::TestInfo> starts;
	void OnStart(const okra::TestInfo &testInfo) override { starts.push_back(testInfo); }

	std::vector<okra::TestInfo> ends;
	void OnEnd(const okra::TestInfo &testInfo, long long c) override { ends.push_back(testInfo); }

	void OnFail(const std::string &message) override {}
};

TEST("when running a test")
{
	auto string_listener = std::make_shared<StringListener>();

	auto subject = okra::TestInfo({"test.cpp", "A test", [](const auto &) {}});
	subject.Run({string_listener});

	ASSERT_EQUAL(1, string_listener->starts.size());
	ASSERT_EQUAL(subject.name, string_listener->starts[0].name);
	ASSERT_EQUAL(1, string_listener->ends.size());
	ASSERT_EQUAL(subject.name, string_listener->ends[0].name);
}