#!/bin/sh

d="$(python rand4.py)"
echo "$d"
echo "$d" | ./p4
