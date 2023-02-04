#!/bin/bash

rm -rf Release-linux/ &&
rm -rf tests/ &&
rm -rf build/ &&
cmake -H. -Bbuild &&
cmake --build build -- -j40
