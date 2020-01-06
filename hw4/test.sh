#!/bin/sh

target1=pa
target2=pa.tle

g++ -std=c++17 -O2 -o a.out $target1.cpp
g++ -std=c++17 -O2 -o b.out $target2.cpp

while true; do
    din="$(python randa.py)"
    dout1="$(printf "$din" | ./a.out)"
    dout2="$(printf "$din" | ./b.out)"

    if [ "$dout1" != "$dout2" ]; then
        echo [INPUT]
        echo "$din"
        echo ===================
        echo [OUTPUT 1]
        echo "$dout1"
        echo -------------------
        echo [OUTPUT 2]
        echo "$dout2"
        echo

        break
    fi

    printf .
done
