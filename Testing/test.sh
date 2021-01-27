#!/bin/bash

if (cd ../ && exec ./build.sh debug install) ; then
	if g++ main.cpp -Wl,-rpath=/usr/local/lib/ -lVirtualPanic -lSDL2 -lGL -lGLEW ; then
		echo "Done!"
		./a.out
		#gdb -ex='set confirm on' -ex=run -ex=quit --args ./a.out		
	fi
fi
