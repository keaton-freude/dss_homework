echo off
dir
cd ./build && cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=RELEASE .. && cmake --build . --config Release && cd ./bin && 7z a DSSHomework.zip .\Release\*