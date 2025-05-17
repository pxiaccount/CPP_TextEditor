#!/bin/bash

SRC_FILE="main.cpp"
OUTPUT="main"

echo "Compiling $SRC_FILE..."
g++ "$SRC_FILE" -lncurses -o "$OUTPUT"
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

clear
echo "Compilation successful. Running editor..."
sleep 0.5

./"$OUTPUT"

tput cnorm

echo
echo "Editor exited."

