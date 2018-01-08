# Okra

[![Windows Build status (Appveyor)](https://ci.appveyor.com/api/projects/status/u80a5vwc2ru4mu0k/branch/master?svg=true)](https://ci.appveyor.com/project/JayBazuzi/okra/branch/master)

[![Linux Build Status (Travis-CI)](https://travis-ci.org/JayBazuzi/Okra.svg?branch=master)](https://travis-ci.org/JayBazuzi/Okra)

## What is it?

Okra is a TDD/BDD testing framework for C++, so highly opinionated that you probably don't want to use it.

- Header only, so it's easy to adopt. (requires C++ 14 and std::experimental::filesystem)

- Individual tests are named with strings, not C++ identifiers.

- Tests are organized by file and directory, not by C++ class.

- Test names are not duplicated between code and file system. DRY.

- Super-fast execution, thanks to compile-time test discovery. Important for microtesting.

- Super-fast iteration when you only change one thing, thanks to one executable per test file.

- There's only one assertion (`AssertEqual()`). You should write your own custom in-domain assertions, e.g. `AssertUserIsLoggedIn()`

- Use CMake and CTests

It's optimized for a hybrid TDD/BDD/ATDD/microtest/test-as-spec approach. For a first stab at what that means, check out https://jbazuzicode.blogspot.com/2017/02/micro-atdd.html

Got an idea or wish? Send a pull request or open an issue or [tweet](https://twitter.com/jaybazuzi).

## Example:

```
// in make_path_relative.cpp
Example("returns the given path relative to the base")
{
    auto result = make_path_relative(R"(C:\foo\)", R"(C:\foo\qux\baz.cpp)");
    AssertEqual(string(R"(qux\baz.cpp)"), result.string());
}
```
