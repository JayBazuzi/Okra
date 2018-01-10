#include "Okra.h"
#include <sstream>

class StringListener : public okra::IListener
{
public:
	std::vector<okra::ExampleInfo> starts;
	void OnStart(const okra::ExampleInfo &exampleInfo) override { starts.push_back(exampleInfo); }

	std::vector<okra::ExampleInfo> ends;
	void OnEnd(const okra::ExampleInfo &exampleInfo, long long c) override { ends.push_back(exampleInfo); }
};

EXAMPLE("when running an example")
{
	auto string_listener = std::make_shared<StringListener>();

	auto subject = okra::ExampleInfo({"example.cpp", "An example", [](const auto &) {}});
    subject.Run({string_listener});

	ASSERT_EQUAL(1, string_listener->starts.size());
    ASSERT_EQUAL(subject.name, string_listener->starts[0].name);
	ASSERT_EQUAL(1, string_listener->ends.size());
    ASSERT_EQUAL(subject.name, string_listener->ends[0].name);
}