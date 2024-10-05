#! /bin/bash

touch ./build && rm -r ./build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B ./build && cmake --build ./build 
./build/rasm < test/test3.asm | tee /tmp/test3.dmp | cut -d '#' -f1 | xxd -r -ps | xxd -g1 
cat /tmp/test3.dmp
