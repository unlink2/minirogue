#!/bin/sh

premake5 gmake && make && ./bin/Debug/mrg "$@"
    # valgrind --leak-check=full --track-origins=yes ./bin/Debug/mrg "$@"

