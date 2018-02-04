#include "Okra.h"

TEST("A test fails when it throws an exception.")
{
	okra::TestInfo subject{"a test", [&] { throw std::exception(); }};
	ASSERT(!subject.Run({}));
}

TEST("A failed assertion throws an AssertionFailedException.")
{
	ASSERT_THROWS(okra::internals::AssertionFailedException, []() { ASSERT(false); });
}
