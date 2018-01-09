#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace okra
{
	template <class T, class U>
	void AssertEqual(const T &t, const U &u, const std::string &message, bool &pass)
	{
		pass = true;
		if (t != u) {
			std::cout << ": " << message << " - assert FAILED - " << t << " != " << u << std::endl;
			pass = false;
		}
	}

	struct ExampleInfo
	{
		const std::string file_path;
		const std::string name;
		const std::function<void(bool &)> body;

		bool Run() const;
	};

	class IListener
	{
	public:
		virtual void OnStart(const ExampleInfo &exampleInfo) = 0;
		virtual void OnEnd(const ExampleInfo &exampleInfo, long long execution_time_us) = 0;
	};

	namespace internals
	{
		template <typename TClock>
		std::pair<long long, bool>
		time_to_execute_microseconds(const std::function<void(bool &OKRA_pass)> &operation)
		{
			auto begin = TClock::now();
			bool pass;
			operation(pass);
			auto end = TClock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			return {duration.count(), pass};
		}

		class ConsoleListener : public IListener
		{
		public:
			void OnStart(const ExampleInfo &exampleInfo) override { std::cout << exampleInfo.name; }
			void OnEnd(const ExampleInfo &exampleInfo, long long execution_time_us) override
			{
				std::cout << " (" << (execution_time_us / 1000.0) << " ms)" << std::endl;
			}
		};

		static std::vector<std::shared_ptr<IListener>> listeners;
	}

	bool ExampleInfo::Run() const
	{
		for (const auto &listener : internals::listeners) {
			listener->OnStart(*this);
		}

		bool pass;
		long long execution_time_us;
		std::tie(execution_time_us, pass) =
		    internals::time_to_execute_microseconds<std::chrono::high_resolution_clock>(body);

		for (const auto &listener : internals::listeners) {
			listener->OnEnd(*this, execution_time_us);
		}

		return pass;
	}

	namespace internals
	{
		class Examples
		{
			std::vector<ExampleInfo> examples;

		public:
			void Add(ExampleInfo exampleInfo) { examples.push_back(exampleInfo); }

			bool RunAll() const
			{
				if (examples.empty()) {
					return false;
				}
				bool pass = true;
				for (const auto &exampleInfo : examples) {
					pass &= exampleInfo.Run();
				}
				return pass;
			}
		};

		Examples allExamples;
	} // namespace internals

	template <class T>
	void RegisterListener()
	{
		okra::internals::listeners.push_back(std::make_shared<T>());
	}
} // namespace okra

#define OKRA_REGISTER_LISTENER(name) OKRA_REGISTER_LISTENER_(name, __COUNTER__)
#define OKRA_REGISTER_LISTENER_(name, counter) OKRA_REGISTER_LISTENER__(name, counter)
#define OKRA_REGISTER_LISTENER__(name, counter)                                                                        \
	OKRA_REGISTER_LISTENER___(name, LISTENER_##counter, ListenerInitializer##counter)
#define OKRA_REGISTER_LISTENER___(name, bodyName, initializerName)                                                     \
	struct initializerName                                                                                         \
	{                                                                                                              \
		initializerName() { okra::RegisterListener<name>(); }                                                  \
	} initializerName##Instance

#ifndef OKRA_DO_NOT_DEFINE_REGISTER_LISTENER
#define REGISTER_LISTENER OKRA_REGISTER_LISTENER
#endif

#define OKRA_EXAMPLE(name) OKRA_EXAMPLE_(name, __COUNTER__)
#define OKRA_EXAMPLE_(name, counter) OKRA_EXAMPLE__(name, counter)
#define OKRA_EXAMPLE__(name, counter) OKRA_EXAMPLE___(name, EXAMPLE_##counter, ExampleInitializer##counter)
#define OKRA_EXAMPLE___(name, bodyName, initializerName)                                                               \
	void bodyName(bool &OKRA_pass);                                                                                \
	struct initializerName                                                                                         \
	{                                                                                                              \
		initializerName() { okra::internals::allExamples.Add({__FILE__, name, bodyName}); }                    \
	} initializerName##Instance;                                                                                   \
	void bodyName(bool &OKRA_pass)

#define OKRA_ASSERT_EQUAL(t1, t2) okra::AssertEqual((t1), (t2), #t1 " == " #t2, OKRA_pass)

#ifndef OKRA_DO_NOT_DEFINE_SHORT_NAMES
#define ASSERT_EQUAL OKRA_ASSERT_EQUAL
#define EXAMPLE OKRA_EXAMPLE
#endif

OKRA_REGISTER_LISTENER(okra::internals::ConsoleListener);

int main(int argc, char **argv) { return okra::internals::allExamples.RunAll() ? 0 : 1; }
