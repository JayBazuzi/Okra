#include "stdafx.h"

#include "Okra.h"
using namespace okra;

Example("the name of the test is based on the path") {
    auto result = okra::get_test_name_from_path(R"(C:\foo)", R"(C:\foo\qux\baz.cpp)");
    AssertEqual(result, R"(qux\baz)");
    
}}