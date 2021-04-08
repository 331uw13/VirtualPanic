#!/bin/bash

objdump -d libVirtualPanic.a -M intel --visualize-jumps=color | less -r

