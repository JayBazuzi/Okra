// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
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

    void Run() const
    {
        cout << file << endl;
        cout << name << endl;
        body();
        cout << endl;
    }
};

class Examples
{
    vector<Example> examples;

public:
    void Add(Example example) { examples.push_back(example); }

    void RunAll() const
    {
        for (const auto& example : examples)
        {
            example.Run();
        }
    }
};

Examples allExamples;

#define Example(name, body) Example_(name, body, __COUNTER__)
#define Example_(name, body, counter) Example__(name, body, counter)
#define Example__(name, body, counter)                                                  \
    struct ExampleInitializer##counter                                                  \
    {                                                                                   \
        ExampleInitializer##counter()                                                   \
        {                                                                               \
            allExamples.Add({__FILE__, name, body});                                     \
        }                                                                               \
    } ExampleInitializer##counter##Instance;                                            \

Example("it multiplies two numbers", []
{
    auto testSubject = Calculator();
    auto result = testSubject.Multiply(6, 9);
    Assert(result == 42);
});

int main(int argc, char** argv)
{
    allExamples.RunAll();
    return 0;
}
