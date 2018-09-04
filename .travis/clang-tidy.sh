#!/bin/bash

result=0
find . -name '*.cpp' -or -name '*.h' | while read file
do
    d=$(diff -u "$file" <(clang-tidy "$file") )
    if ! [ -z "$d" ]; then
        echo "!!! $file not clang-tidy compliant"
        result=1
    fi
done

exit $result
