#!/bin/sh 

premake5 gmake && make && gdb ./bin/Debug/mrg "$@"
