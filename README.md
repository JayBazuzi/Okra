# Okra

## What is it?

Okra is a TDD/BDD testing framework for C++.

## Design goals:

- Avoid duplication. The names of tests or whatever should only appear in one place.

- Use strings to name things. Let you organize code in your language, not in C++ identifier syntax.

- Hierachical test organization. So you can have sections and subsections.

- Low overhead. C# reflection-based test frameworks have enough overhead to be a problem for microtests.

- Short edit/build/test cycle when TDD-ing one class.

- Header-only. Because no one got time to build libraries.

## Some ideas

### Don't have test fixtures or Describe blocks

When people use NUnit and the like, they often name the test fixture and the file name the same. That's duplication. Let's eliminate it. Instead, use the file name, from __FILE__ .

Test runner can collect the file paths of all tests it's going to run and eliminate the common prefix. Then break on path separator and group printed output by directory.

### Easy to run one file of tests

When doing a tight TDD cycle, I am writing tests for a single class. When I change just the tests and the code under test, I don't want to wait to compile or link any other code. So each file of tests should be able to become its own executable program.
