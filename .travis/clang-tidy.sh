#!/bin/bash

result=0
find . -name '*.cpp' -or -name '*.h' | while read file
do
    d=$(diff -u "$file" <(clang-format "$file") )
    if ! [ -z "$d" ]; then
        echo "!!! $file not clang-format compliant"
        result=1
    fi
done

exit $result
