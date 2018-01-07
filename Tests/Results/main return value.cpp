#define OKRA_DO_NOT_DEFINE_EXAMPLE 1
#include "Okra.h"
using namespace okra;
using namespace okra::internals;

void PassingExampleBody(bool &pass) { pass = true; }
ExampleInfo PassingExample{"/passing/example.cpp", "A passing example",
                       PassingExampleBody};

void FailingExampleBody(bool &pass) { pass = false; }
ExampleInfo FailingExample{"/Failing/example.cpp", "A failing example",
                       FailingExampleBody};

#define AssertPassed(operation) AssertEqual(true, operation);
#define AssertFailed(operation) AssertEqual(false, operation);

OKRA_Example("All tests successful") {
  Examples subject;
  subject.Add(PassingExample);
  subject.Add(PassingExample);
  AssertPassed(subject.RunAll());
}

OKRA_Example("A failing test") {
  Examples subject;
  subject.Add(PassingExample);
  subject.Add(FailingExample);
  AssertFailed(subject.RunAll());
}

OKRA_Example("No tests found is a failure") {
  Examples subject;
  AssertFailed(subject.RunAll());
}
