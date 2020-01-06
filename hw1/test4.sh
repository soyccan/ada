#!/bin/sh

\g++ -std=c++17 -o p4 p4.cpp
\g++ -std=c++17 -o p4.0 p4.0.cpp

while true; do
    inp=`python rand4.py`
    out1=`echo "$inp" | ./p4`
    out2=`echo "$inp" | ./p4.0`
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
