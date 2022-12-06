# Onip

*Going to add an image of the Engine editor here when there is a somewhat working version of it*

## About

Onip is a 2D Game Engine/Framework written in C++ and uses OpenGL for the rendering Pipeline. The Project is built by a unit student currently in their first year of studding computer science.

The Goal of the Project is to learn Game Engine architecture and to improve my C++ programming skills while also learning Graphics programming as I would like to support 3D in the future.

## Building/Install

The project uses CMake which should make it very easy to build on any operating system. You'll obviously need to have CMake and a C++ Compiler installed of your choice

### Linux/Mac Steps

Starting in the root directory, open the terminal and enter the following commands
```bash
$ mkdir bin
$ cd bin
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```
Following those commands will build you a **RELEASE Build** of the engine, however, if you want a **DEBUG Build**, just replace the *Release* keyword with *Debug* or just remove the compiler definition.

### Windows

Make sure you have Visual Studio installed as compiling the engine using MSVC compiler is the easiest on this OS.
Open the directory in Visual Studio, then press *Ctr + b* to build the program. The build application will be in the Debug Folder. If you want **Release Build**, the setting that needs to be changed is at the top (currently set to Debug)

## Development Stages

*coming soon …*

## Dependencies

* [glfw](https://www.glfw.org/)
* [glad](https://glad.dav1d.de/)
* [glm](https://github.com/g-truc/glm)
* [stb](https://github.com/nothings/stb)
* [imgui](https://github.com/ocornut/imgui)

## Licensing 

*The following just needs to be included in a LICENCE.txt file wherever you use code from this project:*

Copyright 2022 Ewan Robson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files for Onip Game Engine, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.