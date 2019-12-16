echo off
dir
cd ./build && cmake -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=DEBUG -DOPENSSL_ROOT_DIR=..\\thirdparty\\openssl .. && cmake --build .