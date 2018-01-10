#define OKRA_DO_NOT_DEFINE_EXAMPLE 1
#include "Okra.h"
using namespace okra;
using namespace okra::internals;

void PassingExampleBody(bool &pass) { pass = true; }
ExampleInfo PassingExample{"/passing/example.cpp", "A passing example", PassingExampleBody};

void FailingExampleBody(bool &pass) { pass = false; }
ExampleInfo FailingExample{"/Failing/example.cpp", "A failing example", FailingExampleBody};

void FailInTryCatchExampleBody(bool &pass) { pass = false; }
ExampleInfo FailInTryCatchExample{"/FailInTryCatch/example.cpp", "A failing example", FailInTryCatchExampleBody};

#define AssertPassed(operation) ASSERT_EQUAL(true, operation);
#define AssertFailed(operation) ASSERT_EQUAL(false, operation);

OKRA_EXAMPLE("Multiple successful tests = pass")
{
	Examples subject;
	subject.Add(PassingExample);
	subject.Add(PassingExample);
	AssertPassed(subject.RunAll());
}

OKRA_EXAMPLE("Any test fails = fail")
{
	Examples subject;
	subject.Add(PassingExample);
	subject.Add(FailingExample);
	AssertFailed(subject.RunAll());
}

OKRA_EXAMPLE("No tests found is a failure")
{
	Examples subject;
	AssertFailed(subject.RunAll());
}

OKRA_EXAMPLE("Assertions work in try/catch") { AssertFailed(FailInTryCatchExample.Run({})); }
