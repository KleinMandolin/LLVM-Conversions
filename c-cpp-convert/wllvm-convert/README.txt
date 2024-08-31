
[1] Install bash to run shell scripts:
#
sudo apt-get install bash
#

[2] Run c2llvm-setup.sh to install dependencies (if you're curious, you can open the .sh with a text editor to poke around in there).

Since its installing stuff, run it as sudo. 
Make sure the terminal is in the folder the .sh is in (you can open the terminal from a specific folder in file explorer by hitting the three dots/kebab button next to the search bar at the top, then 'run in terminal'). Run the code below:

#
sudo bash c2llvm-setup.sh
#

Since it just installs dependencies, you shouldn't need to run it again.

[3] Now, you run the c2llvm.sh as a non-root/sudo user (as the folders it create will be protected and difficult to edit/delete otherwise). Run this command with the terminal in the folder containing c2llvm.sh:

#
bash c2llvm.sh
#

[4] Follow the prompts it tells you.

For a specific example, the included test projects have both cmake and makefile style. If you want to do the simple makefile project (it auto-detects which one your project is), you go to the folder containing the Makefile and do ctrl+L then ctrl+C, then in the terminal do ctrl+shift+V and hit enter. For CMake, there are some unique steps you will be prompted to do in the the shell script, but the shell script will walk you through it.

When it is finished compiling, it will try and find the correct .o object file or executable (for the test projects, the makefile will have /obj/main.o and the cmake will have build/myprogram [no filetype suffix on it as it's a unique type]). If it looks like the correct file has been found (for the test projects it will be), then hit y/Y and then go look at your finished .bc and .ll files in the listed directory

NOTE: the c2llvm.sh script was written based on the two included "simple" projects. For unique projects, such as makefile projects that don't create a directory obj/output.o type, you will need to do it manually.

Credits: The c2llvm.sh script was written based off of this simple tutorial here: https://github.com/secure-software-engineering/phasar/wiki/Whole-Program-Analysis-(using-WLLVM)

