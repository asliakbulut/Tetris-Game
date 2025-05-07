#!/bin/bash
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on
cp -f compile_commands.json ../
make
./TetrisCLI
