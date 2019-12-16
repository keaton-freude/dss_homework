echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -host_arch=amd64 -arch=amd64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -test
cd ./build
cmake -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build .