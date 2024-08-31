#!/bin/bash

if [ "$EUID" -ne 0 ]; then
	printf "Please run as root/sudo to install dependencies!\n"
	
	printf "Exiting...\n"
	exit
fi

printf "Installing dependencies...\n"

# not entirely sure if this is required, but for the makefile test
# it wouldn't properly compile (C++ program was missing iostream) so
# included anyway (note, issue only encountered so far on Linux Ubuntu 22.04)
printf "\nFixing g++ errors via reinstall, see shell script for context\n\n"
apt install g++-12

printf "\nInstalling llvm...\n\n"
apt install llvm

printf "\nInstalling cmake...\n\n"
apt install cmake

printf "\nInstalling python3-pip...\n\n"
apt install python3-pip

printf "\nInstalling wllvm...\n\n"
pip install wllvm

printf "\n###\n\nPrereqs and dependencies for c2llvm installed! You can now run c2llvm normally. You (hopefully) will not need to run this c2llvm-setup.sh again.\n\n"
