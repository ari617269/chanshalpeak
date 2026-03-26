#!/bin/bash
gcc -std=c23 main.c -o main
./main ./test.js
rm ./main
