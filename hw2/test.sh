#!/usr/bin/env dash

target1=p4
target2=p4.1

g++ -O2 -std=c++17 -o a.out $target1.cpp
g++ -O2 -std=c++17 -o b.out $target2.cpp

while true; do
    din="$(python rand4.py)"
    dout1="$(printf "$din" | ./a.out)"
    dout2="$(printf "$din" | ./b.out)"

    if [ "$dout1" -lt "$dout2" ] || { [ "$dout1" = -1 ] && [ "$dout1" -ne "$dout2" ]; }; then
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
