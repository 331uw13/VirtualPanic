#!/bin/bash

echo "..."
if (cd .. && ./build.sh install) ; then
	if g++ main.cpp -lVirtualPanic -lGL -lSDL2 -ldl; then
		echo "Done!"
		./a.out
	fi
fi
