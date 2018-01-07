#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include <experimental/filesystem>

#define AssertEqual(t1, t2) AssertEqual_((t1), (t2), #t1 " == " #t2, OKRA_pass)
namespace okra {
template <class T, class U>
void AssertEqual_(const T &t, const U &u, std::string message, bool &pass) {
  pass = true;
  if (t != u) {
    std::cout << ": " << message << " - assert FAILED - " << t << " != " << u
              << std::endl;
    pass = false;
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
get_test_name_from_path(const std::experimental::filesystem::path &base,
                        std::experimental::filesystem::path file) {
  return std::experimental::filesystem::path(
      file.replace_extension().string().substr(base.string().length()));
}

template <typename TClock>
std::pair<long long, bool> time_to_execute_microseconds(
    const std::function<void(bool &OKRA_pass)> &operation) {
  auto begin = TClock::now();
  bool pass;
  operation(pass);
  auto end = TClock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
  return {duration.count(), pass};
}

struct ExampleInfo {
  std::experimental::filesystem::path file;
  std::string name;
  std::function<void(bool &)> body;

  bool Run() const {
    std::cout << name;
    bool pass;
    long long execution_time_us;
    std::tie(execution_time_us, pass) =
        time_to_execute_microseconds<std::chrono::high_resolution_clock>(body);
    std::cout << " (" << (execution_time_us / 1000.0) << " ms)" << std::endl;
    return pass;
  }
};

template <typename TSource, typename TDestination>
inline std::vector<TDestination>
transform(const std::vector<TSource> &input,
          const std::function<TDestination(TSource)> &operation) {
  std::vector<TDestination> result;
  std::transform(input.cbegin(), input.cend(), std::back_inserter(result),
                 operation);
  return result;
}

class Examples {
  std::vector<ExampleInfo> examples;

public:
  void Add(ExampleInfo example) { examples.push_back(example); }

  bool RunAll() const {
    if (examples.empty()) { return false; }
    bool pass = true;
    for (const auto &example : examples) {
      pass &= example.Run();
    }
    return pass;
  }
};

Examples allExamples;
}
}

#define OKRA_EXAMPLE(name) OKRA_EXAMPLE_(name, __COUNTER__)
#define OKRA_EXAMPLE_(name, counter) OKRA_EXAMPLE__(name, counter)
#define OKRA_EXAMPLE__(name, counter)                                          \
  OKRA_EXAMPLE___(name, EXAMPLE_##counter, ExampleInitializer##counter)
#define OKRA_EXAMPLE___(name, bodyName, initializerName)                       \
  void bodyName(bool &OKRA_pass);                                              \
  struct initializerName {                                                     \
    initializerName() {                                                        \
      okra::internals::allExamples.Add({__FILE__, name, bodyName});            \
    }                                                                          \
  } initializerName##Instance;                                                 \
  void bodyName(bool &OKRA_pass)

#ifndef OKRA_DO_NOT_DEFINE_EXAMPLE
#define EXAMPLE OKRA_EXAMPLE
#endif

int main(int argc, char **argv) {
  return okra::internals::allExamples.RunAll() ? 0 : 1;
}
