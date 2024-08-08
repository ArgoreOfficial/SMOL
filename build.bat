@echo off
xmake f --vs=2022
xmake project -k vsxmake -y -m "Debug" -a "x64" ./build