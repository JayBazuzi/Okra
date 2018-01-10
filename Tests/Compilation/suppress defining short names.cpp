#define OKRA_DO_NOT_DEFINE_SHORT_NAMES
#include "Okra.h"
using namespace okra;

OKRA_EXAMPLE("compiles without error")
{
	OKRA_ASSERT_EQUAL(1, 1);
	OKRA_ASSERT_MESSAGE(true, "foo");
}
