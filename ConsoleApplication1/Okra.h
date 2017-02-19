#pragma once

#define AssertEqual(t1, t2) AssertEqual_((t1), (t2), #t1 " == " #t2)
template<class T>
void AssertEqual_(const T& t1, const T& t2, string message)
{
    if (t1 != t2)
    {
        cout << message << " - assert FAILED - " << t1 << " != " << t2 << endl;
    }
}

filesystem::path make_path_relative(const filesystem::path& base, const filesystem::path& file)
{
    return filesystem::path(file.string().substr(base.string().length() + 1));
}

struct Example
{
    filesystem::path file;
    string name;
    function<void(void)> body;

    void Run(filesystem::path base) const
    {
        cout << make_path_relative(base, file) << endl;
        cout << name << endl;
        body();
        cout << endl;
    }
};

filesystem::path GetCommonRootTwo(const filesystem::path& path1, const filesystem::path& path2)
{
    filesystem::path common;
    auto iter1 = path1.begin();
    auto iter2 = path2.begin();
    while (iter1 != path1.end() && iter2 != path2.end())
    {
        if (*iter1 != *iter2) { break; }
        common += *iter1;
        iter1++;
        iter2++;
    }
    return common;
}

filesystem::path GetCommonRootMany(vector<filesystem::path> paths)
{
    return accumulate(
        next(paths.begin()),
        paths.end(),
        paths.front(),
        GetCommonRootTwo);
}

class Examples
{
    vector<Example> examples;

public:
    void Add(Example example) { examples.push_back(example); }

    static filesystem::path GetMaximumSharedFilePrefix(const vector<Example>& examples)
    {
        vector<filesystem::path> paths;
        transform(examples.cbegin(), examples.cend(), back_inserter(paths), [](const Example& _) {return _.file; });
        return GetCommonRootMany(paths);
    }

    void RunAll() const
    {
        auto maximumSharedFilePrefix = GetMaximumSharedFilePrefix(examples);
        for (const auto& example : examples)
        {
            example.Run(maximumSharedFilePrefix);
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
