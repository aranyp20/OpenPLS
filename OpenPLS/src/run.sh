#!/bin/sh

cd builddir
if [ -e main.exe ]
then
rm main.exe
fi
g++ -std=c++11 --prefix=builddir -c ../*.cpp
g++ *.o -o main.exe -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW
./main.exe
cd ..