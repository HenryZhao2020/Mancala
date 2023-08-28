@echo off
mkdir Build
cd Build
qmake "../Mancala.pro"
mingw32-make
"release/Mancala"