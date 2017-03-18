#!/bin/bash

echo Compiling...
g++ -o Source.o -c Source.cpp -O0
g++ -o LCDDriver.o -c LCDDriver.cpp -O0
g++ -o terminus.o -c terminus.cpp -O0
g++ -o LCDBenchMark.o -c LCDBenchMark.cpp -O0

echo Linking...
g++ -o LCDApp Source.o LCDDriver.o terminus.o LCDBenchMark.o -lwiringPi

