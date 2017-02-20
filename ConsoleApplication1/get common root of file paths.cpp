#include "stdafx.h"

#include "Okra.h"

Example("common root of two related paths") {
  auto result = get_common_root({R"(C:\foo\bar.cpp)", R"(C:\foo\qux\baz.cpp)"});
  AssertEqual(result, R"(C:\foo)");
}
}

Example("same path just returns directory") {
  auto result = get_common_root({R"(C:\foo\bar.cpp)",
                                 R"(C:\foo\bar.cpp)"});
  AssertEqual(result, R"(C:\foo)");
}
}

Example("common root of vector of paths") {
  auto result = get_common_root({R"(C:\foo\bar.cpp)", R"(C:\foo\qux\baz.cpp)"});
  AssertEqual(result, R"(C:\foo)");
}
}
