#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include <experimental/filesystem>

#define AssertEqual(t1, t2) AssertEqual_((t1), (t2), #t1 " == " #t2)
namespace okra {
template <class T, class U>
void AssertEqual_(const T &t, const U &u, std::string message) {
  if (t != u) {
    std::cout << ": " << message << " - assert FAILED - " << t << " != " << u
              << std::endl;
  }
}
namespace internals {
inline std::experimental::filesystem::path
get_common_root2(const std::experimental::filesystem::path &path1,
                 const std::experimental::filesystem::path &path2) {
  std::experimental::filesystem::path common;
  auto iter1 = path1.begin();
  auto iter2 = path2.begin();
  while (iter1 != path1.end() && iter2 != path2.end()) {
    if (*iter1 != *iter2) {
      break;
    }
    common /= *iter1;
    iter1++;
    iter2++;
  }

  return common;
}

inline std::experimental::filesystem::path
get_common_root(std::vector<std::experimental::filesystem::path> paths) {
  return std::accumulate(next(paths.begin()), paths.end(),
                         paths.front().remove_filename(), get_common_root2);
}

inline std::experimental::filesystem::path
get_test_name_from_path(const std::experimental::filesystem::path &base,
                        std::experimental::filesystem::path file) {
  return std::experimental::filesystem::path(
      file.replace_extension().string().substr(base.string().length()));
}

struct Example {
  std::experimental::filesystem::path file;
  std::string name;
  std::function<void(void)> body;

  void Run(std::experimental::filesystem::path base) const {
    std::cout << get_test_name_from_path(base, file) << " - " << name;
    body();
    std::cout << std::endl;
  }
};

template <typename TSource, typename TDestination>
inline std::vector<TDestination>
transform(const std::vector<TSource> &input,
          const std::function<TDestination(TSource)> operation) {
  std::vector<TDestination> result;
  std::transform(input.cbegin(), input.cend(), std::back_inserter(result),
                 operation);
  return result;
}

class Examples {
  std::vector<Example> examples;

public:
  void Add(Example example) { examples.push_back(example); }

  void RunAll() const {
    auto maximumSharedFilePrefix =
        get_common_root(transform<Example, std::experimental::filesystem::path>(
            examples, [](const auto &_) { return _.file; }));

    for (const auto &example : examples) {
      example.Run(maximumSharedFilePrefix);
    }
  }
};

Examples allExamples;
}
}

#define OKRA_Example(name) OKRA_Example_(name, __COUNTER__)
#define OKRA_Example_(name, counter) OKRA_Example__(name, counter)
#define OKRA_Example__(name, counter)                                          \
  OKRA_Example___(name, Example##counter, ExampleInitializer##counter)
#define OKRA_Example___(name, bodyName, initializerName)                       \
  void bodyName();                                                             \
  struct initializerName {                                                     \
    initializerName() {                                                        \
      okra::internals::allExamples.Add({__FILE__, name, bodyName});            \
    }                                                                          \
  } initializerName##Instance;                                                 \
  void bodyName()

#ifndef OKRA_DO_NOT_DEFINE_EXAMPLE
#define Example OKRA_Example
#endif

#ifdef OKRA_MAIN

int main(int argc, char **argv) {
  okra::internals::allExamples.RunAll();
  return 0;
}

#endif
