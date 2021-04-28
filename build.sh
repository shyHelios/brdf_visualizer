#!/bin/bash

# Compiles everything

mkdir bin

mkdir bin/debug
mkdir bin/release
mkdir bin/reldbginfo

cd bin/debug
cmake ../.. -DCMAKE_BUILD_TYPE=Debug
make brdfviz -j$(nproc)
make brdfviz_benchmark -j$(nproc)
cd ../..

cd bin/release
cmake ../.. -DCMAKE_BUILD_TYPE=Release
make brdfviz -j$(nproc)
make brdfviz_benchmark -j$(nproc)
cd ../..

cd bin/reldbginfo
cmake ../.. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make brdfviz -j$(nproc)
make brdfviz_benchmark -j$(nproc)
cd ../..

./copydata.sh