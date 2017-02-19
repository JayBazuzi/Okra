#include "stdafx.h"

#include "Okra.h"

Example("the name of the test is based on the path") {
    auto result = get_test_name_from_path(R"(C:\foo)", R"(C:\foo\qux\baz.cpp)");
    AssertEqual(result.string(), string(R"(qux\baz)"));
}}
