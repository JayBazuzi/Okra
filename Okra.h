#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace okra
{
	class IListener;
	struct TestInfo;

	namespace internals
	{
		template <typename TClock>
		long long time_to_execute_microseconds(const std::function<void()> &operation);
		static std::vector<std::shared_ptr<IListener>> listeners;
	}

	class IListener
	{
	public:
		virtual void OnStart(const TestInfo &testInfo) = 0;
		virtual void OnEnd(const TestInfo &testInfo, long long execution_time_us) = 0;
	};

	struct TestInfo
	{
		const std::string file_path;
		const std::string name;
		const std::function<void(bool &)> body;

		bool Run(const std::vector<std::shared_ptr<IListener>> &listeners) const;
	};

	bool TestInfo::Run(const std::vector<std::shared_ptr<IListener>> &listeners) const
	{
		for (const auto &listener : listeners) {
			listener->OnStart(*this);
		}

		bool pass;
		long long execution_time_us;
		execution_time_us = internals::time_to_execute_microseconds<std::chrono::high_resolution_clock>([&]() {
			try
			{
				body(pass);
			}
			catch (...)
			{
				pass = false;
			}
		});

		for (const auto &listener : listeners) {
			listener->OnEnd(*this, execution_time_us);
		}

		return pass;
	}

	template <class T>
	void RegisterListener()
	{
		okra::internals::listeners.push_back(std::make_shared<T>());
	}

	namespace internals
	{
		class AssertionFailedException
		{
		};

		void Fail(const std::string &message)
		{
			std::cout << ": "
			          << " - assert FAILED - " << std::endl
			          << message << std::endl;
			throw AssertionFailedException();
		}

		void AssertMessage(bool condition, const std::string &message, bool &pass)
		{
			pass = true;
			if (!condition) {
				pass = false;
				Fail(message);
			}
		}

		template <class T1, class T2>
		void AssertEqual(
		    const T1 &t1, const T2 &t2, const std::string &t1String, const std::string &t2String, bool &pass)
		{
			std::stringstream stringstream;
			stringstream << "EXPECTED: " << t1 << "(" << t1String << ")" << std::endl;
			stringstream << "ACTUAL  : " << t2 << "(" << t2String << ")" << std::endl;
			AssertMessage(t1 == t2, stringstream.str(), pass);
		}

		template <typename TClock>
		long long time_to_execute_microseconds(const std::function<void()> &operation)
		{
			auto begin = TClock::now();
			operation();
			auto end = TClock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			return duration.count();
		}

		class OStreamListener : public IListener
		{
			std::ostream &ostream;

		public:
			OStreamListener(std::ostream &ostream)
			    : ostream(ostream)
			{
			}
			void OnStart(const TestInfo &testInfo) override { ostream << testInfo.name; }
			void OnEnd(const TestInfo &testInfo, long long execution_time_us) override
			{
				ostream << " (" << (execution_time_us / 1000.0) << " ms)" << std::endl;
			}
		};

		class ConsoleListener : public OStreamListener
		{
		public:
			ConsoleListener()
			    : OStreamListener(std::cout)
			{
			}
		};

		class Tests
		{
			std::vector<TestInfo> tests;

		public:
			void Add(TestInfo testInfo) { tests.push_back(testInfo); }

			bool RunAll() const
			{
				if (tests.empty()) {
					return false;
				}
				bool pass = true;
				for (const auto &testInfo : tests) {
					pass &= testInfo.Run(listeners);
				}
				return pass;
			}
		};

		Tests allTests;
	} // namespace internals
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

#define OKRA_TEST(name) OKRA_TEST_(name, __COUNTER__)
#define OKRA_TEST_(name, counter) OKRA_TEST__(name, counter)
#define OKRA_TEST__(name, counter) OKRA_TEST___(name, TEST_##counter, TestInitializer##counter)
#define OKRA_TEST___(name, bodyName, initializerName)                                                                  \
	void bodyName(bool &OKRA_pass);                                                                                \
	struct initializerName                                                                                         \
	{                                                                                                              \
		initializerName() { okra::internals::allTests.Add({__FILE__, name, bodyName}); }                       \
	} initializerName##Instance;                                                                                   \
	void bodyName(bool &OKRA_pass)

#define OKRA_ASSERT_MESSAGE(condition, message) okra::internals::AssertMessage((condition), message, OKRA_pass)
#define OKRA_ASSERT(condition) OKRA_ASSERT_MESSAGE((condition), #condition)
#define OKRA_ASSERT_EQUAL(t1, t2) okra::internals::AssertEqual((t1), (t2), #t1, #t2, OKRA_pass)

#ifndef OKRA_DO_NOT_DEFINE_SHORT_NAMES
#define ASSERT_MESSAGE OKRA_ASSERT_MESSAGE
#define ASSERT_EQUAL OKRA_ASSERT_EQUAL
#define ASSERT OKRA_ASSERT
#define TEST OKRA_TEST
#define REGISTER_LISTENER OKRA_REGISTER_LISTENER
#endif

OKRA_REGISTER_LISTENER(okra::internals::ConsoleListener);

int main(int argc, char **argv) { return okra::internals::allTests.RunAll() ? 0 : 1; }
