#!/bin/bash

echo "..."
if g++ main.cpp -o demo -lVirtualPanic -lGL -lSDL2 -ldl ; then
	echo "Done!"
fi
