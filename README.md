# Okra

## What is it?

Okra is a TDD/BDD testing framework for C++.

## Design goals:

- Avoid duplication. The names of tests or whatever should only appear in one place.

- Use strings to name things. Let you organize code in your langauge, not in C++ identifier syntax.

- Hierachical test organization. So you can have sections and subsections.

- Low overhead. C# reflection-based test frameworks have enough overhead to be a problem for microtests.

- Short edit/build/test cycle when TDD-ing one class.
