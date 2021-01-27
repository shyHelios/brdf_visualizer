#!/bin/bash

# copies data into binary folder 
# Set this script as post-compile hook, to keep things updated

# Remove old data
rm bin/debug/data -r
rm bin/release/data -r
rm bin/reldbginfo/data -r

# Remove old shaders
rm bin/debug/Shaders -r
rm bin/release/Shaders -r
rm bin/reldbginfo/Shaders -r

# Copy new data
cp data bin/debug -r 
cp data bin/release -r
cp data bin/reldbginfo -r

# Copy new shaders
cp Shaders bin/debug -r
cp Shaders bin/release -r
cp Shaders bin/reldbginfo -r