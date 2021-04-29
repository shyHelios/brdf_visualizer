# Welcome to BRDF vizualizer!

This is multiplatform BRDF visualizer, created in C++.

# Compiling

BRDF visualizer should be compileable on Linux and Windows.

## Linux

For Linux, CMake is required. I have tested it on Manjaro 5.10, with gcc 10.2.0. You have to install embree using your favorite package manager. To compile this
project, you can simply call *buildReleaseOnly.sh* in root directory. If you want to compile the project on your own, I guess you know, what are you doing. But
beware, you have to copy Shaders and data folders into the binary folder.

## Windows

On windows, there can be problems with Embree compilation with GCC. So I have created solution for MSVC2019 and I have provided precompiled dependent libraries.
If you open solution and compile it, you should be good to go. Data and shaders are copied automatically

# Usage

In main window, there is visualization of current BRDF function and in the window on the right, this BRDF function is used for rendering. You can change
parameters, select BRDF functions, select different sampling methods and also try furnace test. You can also save screenshots of both render and BRDF function
visualization. In rendering window, feel free to load some model (I have provided a few) or change environment map (You have to get yourself one on your own. I
recommend [https://hdrihaven.com](https://hdrihaven.com))
