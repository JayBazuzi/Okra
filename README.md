# Okra

## What is it?

Okra is a TDD/BDD testing framework for C++.

It's optimized for a hybrid TDD/BDD/ATDD/microtest approach. I should probably write something about what I mean by that.

Got an idea or wish? Send a pull request.

## Design goals:

- Avoid duplication. The names of tests or whatever should only appear in one place.

- Use strings to name things. Let you organize code in your language, not in C++ identifier syntax.

- Hierachical test organization. So you can have sections and subsections.

- Low overhead. C# reflection-based test frameworks have enough overhead to be a problem for microtests.

- Short edit/build/test cycle when TDD-ing one class.

- Header-only. Because no one got time to build libraries.

## Some thoughts

### Use files instead of  test fixtures or Describe blocks

When people use NUnit and the like, they often name the test fixture and the file name the same. That's duplication. Let's eliminate it. Instead, use the file name, from \_\_FILE\_\_.

Test runner can collect the file paths of all tests it's going to run and eliminate the common prefix. Then break on path separator and group printed output by directory.

### Easy to run one file of tests

When doing a tight TDD cycle, I am writing tests for a single class. When I change just the tests and the code under test, I don't want to wait to compile or link any other code. So each file of tests should be able to become its own executable program.

### How fast can tests be?

TDD afficionados often say that unit tests should be around 1ms. 

I learned unit testing with NUnit, which uses reflection to discover and execute tests. I've seen a lot of tests in the world that are pretty slow -- slow enough that NUnit's overhead didn't really matter. 

I suspect we can do better than 1ms if we write tests for fully context-neutral code. Those tests look something like:

```
auto testSubject = Foo(...);
auto result = testSubject.Bar(...);
Assert(result...)
```

In the success case, all we're doing is:

1. Creating an object and calling it's ctor
2. calling a method
3. comparing a result

That seems like something computers should be really good at, right? Can that run in 0.1ms or less? If so, the overhead of the test framework, both at compile and execution time, will matter a lot.

TODO: performance analysis of these kinds of microtests in C++

### In-domain assertions

Because I like test-as-spec, where tests are first and foremost human-readable specification, I want my assertions to read well.

Many BDD systems let you write something like:

   result.users.single().is_expired.Should.be.true
   
And while it's neat that this is English, that's still a computer-sciencey expression. If I was writing a spec in a non-code document, I might want to write something like "then the user should be expired".

Gherkin lets you write english like that, but then you need extra code to translate between test language and real code. I want to avoid that.

I like the way C#'s Fluent Assertions is extensible, so you can write `Should().BeExpired()`. If we're going to commit to extensibility of assertions, then the built-in assertions should be minimal (maybe just one, which takes a boolean).
