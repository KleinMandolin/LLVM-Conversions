#!/bin/bash

if [ "$EUID" = 0 ]; then
	printf "Please run normally, not as root/sudo!\n"
	
	printf "Exiting...\n"
	exit
fi

export LLVM_COMPILER=clang

read -p "Please enter path to FOLDER containing Makefile/CMakeLists.txt.

You can get this by navigating to the folder containing the file, pressing ctrl+L, ctrl+C and then in here pressing ctrl+shift+V.

Directory: " -r directory

cd "$directory"

printf "\n"

# if makefile found
if [ -f Makefile ]; then
	# if makefile detected in project directory, proceed with makefile style compilation
	printf "Makefile found!\n\n"
	
	# make with wllvm compilers
	make CXX=wllvm++ CC=wllvm
	
	itemfulldir=$(find . -name *.o -type f | head -1)
	item=$(basename "$itemfulldir")
	itemdir=$(dirname "$itemfulldir")
	printf "\n\nFound output > %s < at project directory %s\n\n" "$item" "$itemdir"
	cd "$itemdir"
	currdir=""$(pwd)""
	printf "Is this the compiled object you want LLVM bitcode from? (y/n) "
	read answer
	
	# if detected output .o file is correct one to convert to .ll LLVM IR, convert
	if [ "$answer" != "${answer#[Yy]}" ]; then
		printf "Extracting bitcode and IR to \n%s\n" "$currdir"
		# create .bc from output item
		extract-bc "$item"
		bitcode=$(basename $(find . -maxdepth 1 -name *.bc -type f | head -1))
		# create .ll from .bc file
		llvm-dis "$bitcode" -o OUTPUT.ll
	# else user must manually convert correct output object
	else
		printf "Then you will need to manually convert your object to bitcode. You will need to go to the folder containing the output object file for you project, then run
>   extract-bc NAME.o
and then locate the output bitcode (usually original NAME.o.bc) and run
>   llvm-dis NAME.o.bc -o NEWNAME.ll
You will then be able to read the output .ll file or run the output .bc file with
>   lli NAME.bc\n"
	fi

# else look for cmake file
else
	printf "No Makefile found, looking for CMakeLists.txt...\n\n"
	# if cmake file found instead, proceed with cmake compilation
	if [ -f CMakeLists.txt ]; then
		printf "CMakeLists.txt found!\n\n"
		# if cmake build directory exists (assumes already compiled)
		if [ -d build ]; then
			printf "A build directory already exists! Canceling compile. To run this program on a CMake project, make sure there is no preexisting build folder.\n"
			printf "Exiting...\n"
			exit
		# if clean cmake project directory (not already compiled)
		else
			# user input required for cmake, edit cmakelists to change compiler settings
			printf "\n!!! Please go into you CMakeLists.txt and make sure the fields below match:
>   set(CMAKE_C_COMPILER wllvm)
>   set(CMAKE_CXX_COMPILER wllvm++)
and SAVE ANY CHANGES before continuing here. If you don't, the compiled project can't be converted to LLVM bitcode.\n"
			printf "Did you change the compiler fields and save your CMakeLists.txt? (y/n) "
			read answer
			if [ "$answer" = "${answer#[Yy]}" ]; then
				printf "\nYou need to go do that.\n"
				exit
			fi
			printf "Are you sure? You're gonna have to delete the build folder and start over if not. (y/n) "
			read answer
			if [ "$answer" = "${answer#[Yy]}" ]; then
				printf "\nYou need to go do that.\n"
				exit
			fi
			# make objects
			mkdir build/
			cd build/
			cmake ..
			make
			# find output item
			itemfulldir=$(find . -maxdepth 1 \( ! -name "*.*" -a ! -name "Makefile" \) -type f | head -1 )
			item=$(basename "$itemfulldir")
			currdir=""$(pwd)""
			printf "\n\nFound output > %s < at project_directory/build\n\n" "$item"
			printf "Is this the compiled object you want LLVM bitcode from? (y/n) "
			read answer
			# if correct file to convert
			if [ "$answer" != "${answer#[Yy]}" ]; then
				printf "Extracting bitcode and IR to \n%s\n" "$currdir"
				# create .bc from output item
				extract-bc "$item"
				bitcode=$(basename $(find . -maxdepth 1 -name *.bc -type f | head -1))
				# create .ll from .bc file
				llvm-dis "$bitcode" -o OUTPUT.ll
			# if not correct file to convert
			else
				printf "Then you will need to manually convert your object to bitcode. You will need to go to the folder containing the output executable file for your project, then run
>   extract-bc NAME
and then locate the output bitcode and run
>   llvm-dis NAME.bc -o NAME.ll
You will then be able to read the output .ll file or run the output .bc file with
>   lli NAME.bc\n"
			fi
		fi
	
	else
		printf "No CMakeLists.txt found!\n"
		
		printf "\nPlease make sure your project has a Makefile or CMakeLists.txt for this to work! Check that your input matches the directory of the folder containing this file. You can get the correct folder, assuming Linux OS, by doing ctrl+L in the folder with the file and ctrl+C then, here in the terminal, SHIFT+ctrl+V\n"
		
		printf "\nExiting...\n"
		exit
	fi
fi
