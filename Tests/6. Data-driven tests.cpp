#include "Okra.h"

// #define OKRA_TEST(name) OKRA_TEST_(name, __COUNTER__)
// #define OKRA_TEST_(name, counter) OKRA_TEST__(name, counter)
// #define OKRA_TEST__(name, counter) OKRA_TEST___(name, TEST_##counter, TestInitializer##counter)
// #define OKRA_TEST___(name, bodyName, initializerName) \
// 	void bodyName();                                                                                               \
// 	struct initializerName                                                                                         \
// 	{                                                                                                              \
// 		initializerName() { okra::internals::allTests.Add({name, bodyName}); }                                 \
// 	} initializerName##Instance;                                                                                   \
// 	void bodyName()

#define DATA_DRIVEN_TEST(values, name, arguments) DATA_DRIVEN_TEST_((values), (name), (arguments), __COUNTER__)
#define DATA_DRIVEN_TEST_(values, name, arguments, counter) DATA_DRIVEN_TEST__(values, name, arguments, counter)
#define DATA_DRIVEN_TEST___(values, name, arguments, counter)                                                          \
	DATA_DRIVEN_TEST____(values,                                                                                   \
	                     name,                                                                                     \
	                     arguments,                                                                                \
	                     DATA_DRIVEN_TEST_##counter,                                                               \
	                     DATA_DRIVEN_TEST_BODY_##counter,                                                          \
	                     DataDrivenTestInitializer##counter)
#define DATA_DRIVEN_TEST____(values, name, arguments, bodyName, bodyBodyName, initializerName)                         \
	void bodyName();                                                                                               \
	struct initializerName                                                                                         \
	{                                                                                                              \
		initializerName() { okra::internals::allTests.Add({name, bodyName}); }                                 \
	} initializerName##Instance;                                                                                   \
	void bodyBodyName(arguments);                                                                                  \
	void bodyName()                                                                                                \
	{                                                                                                              \
		for (const auto &value : {1, 2, 3}) {                                                                  \
			bodyBodyName(value);                                                                           \
		}                                                                                                      \
	}                                                                                                              \
	void bodyBodyName(arguments)

DATA_DRIVEN_TEST({1, 2, 3}, "foo", (int i)) { ASSERT_EQUAL(i, 4); }

// void TEST_1_BODY(int i) { ASSERT_EQUAL(i, 4); }

// void TEST_1()
// {
// 	for (const auto &value : {1, 2, 3}) {
// 		TEST_1_BODY(value);
// 	}
// }