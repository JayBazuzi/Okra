#define APPROVALS_OKRA
#include "ApprovalTests.v.1.0.0.hpp"

TEST("TestNameMayNotContainsSpaces") { Approvals::verify("foo"); }
