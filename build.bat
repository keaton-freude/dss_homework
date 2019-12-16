echo off
cd ./build
cmake -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build .