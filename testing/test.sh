#!/bin/bash

if (cd .. && ./build.sh debug); then
	if gcc main.c -L/home/eeiuwie/Dev/VirtualPanic -lVirtualPanic -lGL -lglfw -ldl -lm -o vpanic_test; then
		ldd vpanic_test
		./vpanic_test
	fi
fi




