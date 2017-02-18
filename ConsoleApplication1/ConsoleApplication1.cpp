// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <functional>
#include <vector>
#include <string>
using namespace std;

class Calculator
{
public:
    int Multiply(int x, int y) { return x * y; }
};

void Assert(bool b)
{
    cout << "assertion " << (b ? "pass": "fail") << endl;
}

struct Example
{
    string file;
    string name;
    function<void(void)> body;
};

class Examples
{
    vector<Example> examples;

    void RunOne(const Example & example) const
    {
        cout << example.file << endl;
        cout << '\t' << example.name << endl;
        example.body();
        cout << endl;
    }

public:
    void Add(Example example) { examples.push_back(example); }

    void RunAll() const
    {
        for (const auto& example : examples)
        {
            RunOne(example);
        }
    }
};

Examples allExamples;

#define Example(name) Example_(name, __COUNTER__)
#define Example_(name, counter) Example__(name, counter)
#define Example__(name, counter)                                                        \
    void Example##counter();                                                            \
    struct ExampleInitializer##counter                                                  \
    {                                                                                   \
        ExampleInitializer##counter()                                                   \
        {                                                                               \
            allExamples.Add({__FILE__, name, Example##counter});                        \
        }                                                                               \
    } ExampleInitializer##counter##Instance;                                            \
    void Example##counter()

Example("it multiplies two numbers")
{
    auto testSubject = Calculator();
    auto result = testSubject.Multiply(6, 9);
    Assert(result == 42);
};

int main(int argc, char** argv)
{
    allExamples.RunAll();
    return 0;
}

