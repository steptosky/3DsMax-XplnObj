# The Plugin for the 3DsMax that allows you to export to X-Plane obj format.
- The plugin is distributed under 
[BSD (3-Clause)](http://opensource.org/licenses/BSD-3-Clause) 
license for more information read the [license](license.txt) file.
- [The X-Plane official website](http://www.x-plane.com/)
- [The X-Plane obj specification](http://developer.x-plane.com/?article=obj8-file-format-specification)
- The plugin supports 9-2017 3Ds Max (x64 only)
- Minimum C++ standard is 14

## Warning 
The importing is still under developing and does not work correctly!

## Dependencies
- [CMake 3.7.0+](https://cmake.org/)
- [Conan 0.24+](https://www.conan.io) - dependency tool
- [StepToSky X-Plane obj library](https://github.com/steptosky/XplnObj) - used through the conan
- 3Ds Max SDK - Usually you can find it on 3Ds Max installation cd or image

## Build
- The release [check list](doc/release-checklist.md) - is used for producing the releases.
- You have to get the 3DsMax sdk and then put it into conan repository, 
see [this](https://bitbucket.org/steptosky/conan-recipe-3dsmax-sdk) for
more information.
- Adjust the file [conanfile.txt](conanfile.txt), you can comment 
unnecessary 3DsMax sdk version with *#*. 
It also will turn off appropriate targets.
```
#3DsMaxSdk2009/last@steptosky/stable   will not be used
3DsMaxSdk2010/last@steptosky/stable
```
- Create the buld folder ```mkdir build``` and go in ```cd build```
- Use ```conan install --build=outdated``` to get all necessary dependencies.
- Use ```cmake -G "Visual Studio 14 Win64" ../ -DCMAKE_INSTALL_PREFIX=../output```
for generation the visual studio 2015 project.
- Use ```cmake --build . --target install --config Release``` 
for building the project with cmake or use visual studio.
- You can make a *.bat* file and copy/paste and adjust the following script there.  
You will need to define the profiles or replace it with your ones.
  
```
::==========================================================
@echo off
set dir="msvc"
if not exist %dir% mkdir %dir%
cd %dir%
::==========================================================
call conan install .. --profile vs2015-Release -g cmake_multi --build=outdated
call conan install .. --profile vs2015-Debug -g cmake_multi --build=outdated
::==========================================================
call cmake -G "Visual Studio 14 Win64" ../ ^
	-DCMAKE_INSTALL_PREFIX=../output ^
	-DUSE_CONAN_CMAKE_MULTI=ON 
::call cmake --build . --target install --config Release
::call cmake --build . --target install --config Debug
cd ../
::==========================================================
```
- See also [Dev notes](doc/dev-notes.md)

## Copyright
Copyright (c) 2017, StepToSky team. All rights reserved.  
[www.steptosky.com](http://www.steptosky.com/)