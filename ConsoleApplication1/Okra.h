#pragma once

#define AssertEqual(t1, t2) AssertEqual_((t1), (t2), #t1 " == " #t2)
namespace okra
{
template <class T, class U>
void AssertEqual_(const T &t, const U &u, string message) {
  if (t != u) {
    cout << message << " - assert FAILED - " << t << " != " << u << endl;
  }
}

inline filesystem::path get_common_root2(const filesystem::path &path1,
    const filesystem::path &path2) {
    filesystem::path common;
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

inline filesystem::path get_common_root(vector<filesystem::path> paths) {
    return accumulate(next(paths.begin()), paths.end(),
        paths.front().remove_filename(), get_common_root2);
}

inline filesystem::path get_test_name_from_path(const filesystem::path &base,
    filesystem::path file) {
    return filesystem::path(
        file.replace_extension().string().substr(base.string().length() + 1));
}

struct Example {
  filesystem::path file;
  string name;
  function<void(void)> body;

  void Run(filesystem::path base) const {
    cout << get_test_name_from_path(base, file) << " - " << name;
    body();
    cout << endl;
  }
};

template <typename TSource, typename TDestination>
inline vector<TDestination>
transform(const vector<TSource> &input,
          function<TDestination(TSource)> operation) {
  vector<TDestination> result;
  std::transform(input.cbegin(), input.cend(), back_inserter(result),
                 operation);
  return result;
}

class Examples {
  vector<Example> examples;

public:
  void Add(Example example) { examples.push_back(example); }

  void RunAll() const {
    auto maximumSharedFilePrefix =
        get_common_root(transform<Example, filesystem::path>(
            examples, [](const auto &_) { return _.file; }));

    for (const auto &example : examples) {
      example.Run(maximumSharedFilePrefix);
    }
  }
};

// consider using __attribute__((weak)) and maybe #pragma comment(linker,
// "/alternatename:_pWeakValue=_pDefaultWeakValue")
__declspec(selectany) Examples allExamples;
}

#define OKRA_Example(name) OKRA_Example_(name, __COUNTER__)
#define OKRA_Example_(name, counter) OKRA_Example__(name, counter)
#define OKRA_Example__(name, counter)                                          \
  OKRA_Example___(name, Example##counter, ExampleInitializer##counter)
#define OKRA_Example___(name, bodyName, initializerName)                       \
  namespace {                                                                  \
  void bodyName();                                                             \
  struct initializerName {                                                     \
    initializerName() { okra::allExamples.Add({__FILE__, name, bodyName}); }   \
  } initializerName##Instance;                                                 \
  void bodyName()

#ifndef OKRA_DO_NOT_DEFINE_EXAMPLE
# define Example OKRA_Example
#endif

#ifdef OKRA_MAIN

int main(int argc, char **argv) {
  okra::allExamples.RunAll();
  return 0;
}

#endif