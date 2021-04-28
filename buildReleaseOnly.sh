#!/bin/bash

mkdir bin/release

cd bin/release
cmake ../.. -DCMAKE_BUILD_TYPE=Release
make brdfviz -j$(nproc)
make brdfviz_benchmark -j$(nproc)
cd ../..

./copydata.sh