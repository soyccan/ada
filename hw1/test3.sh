#!/bin/sh

\g++ -std=c++17 -O3 -o p3 p3.cpp
\g++ -std=c++17 -O3 -o p3.tle p3.tle.cpp

while true; do
    printf "-"
    inp=`python rand3.py`
    out1=`echo "$inp" | ./p3`
    out2=`echo "$inp" | ./p3.tle`
    if [ "$out1" != "$out2" ]; then
        echo "$inp"
        echo -------------
        echo $out1
        echo =============
        echo $out2
        break
    fi
#     diff p4.out p4.0.out
#     if [ "$?" != "0" ]; then
#         break
#     fi
done
