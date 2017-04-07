#include "Okra.h"
using namespace okra;
using namespace okra::internals;

Example("common root of two related paths") {
  auto result = get_common_root({R"(/foo/bar.cpp)", R"(/foo/qux/baz.cpp)"});
  AssertEqual(result, R"(/foo)");
}

Example("same path just returns directory") {
  auto result = get_common_root({R"(/foo/bar.cpp)",
                                 R"(/foo/bar.cpp)"});
  AssertEqual(result, R"(/foo)");
}

Example("common root of vector of paths") {
  auto result = get_common_root({R"(/foo/bar.cpp)", R"(/foo/qux/baz.cpp)"});
  AssertEqual(result, R"(/foo)");
}
