# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Install\CLion 2019.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Install\CLion 2019.2.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\projects\CLionProjects\myc1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\projects\CLionProjects\myc1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/myc1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myc1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myc1.dir/flags.make

CMakeFiles/myc1.dir/dev-Tan/main.cpp.obj: CMakeFiles/myc1.dir/flags.make
CMakeFiles/myc1.dir/dev-Tan/main.cpp.obj: CMakeFiles/myc1.dir/includes_CXX.rsp
CMakeFiles/myc1.dir/dev-Tan/main.cpp.obj: ../dev-Tan/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\projects\CLionProjects\myc1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myc1.dir/dev-Tan/main.cpp.obj"
	D:\Install\CodeBlocks\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\myc1.dir\dev-Tan\main.cpp.obj -c D:\projects\CLionProjects\myc1\dev-Tan\main.cpp

CMakeFiles/myc1.dir/dev-Tan/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myc1.dir/dev-Tan/main.cpp.i"
	D:\Install\CodeBlocks\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\projects\CLionProjects\myc1\dev-Tan\main.cpp > CMakeFiles\myc1.dir\dev-Tan\main.cpp.i

CMakeFiles/myc1.dir/dev-Tan/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myc1.dir/dev-Tan/main.cpp.s"
	D:\Install\CodeBlocks\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\projects\CLionProjects\myc1\dev-Tan\main.cpp -o CMakeFiles\myc1.dir\dev-Tan\main.cpp.s

CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.obj: CMakeFiles/myc1.dir/flags.make
CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.obj: CMakeFiles/myc1.dir/includes_C.rsp
CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.obj: ../thirdParty/sqlite3/sqlite3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\projects\CLionProjects\myc1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.obj"
	D:\Install\CodeBlocks\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\myc1.dir\thirdParty\sqlite3\sqlite3.c.obj   -c D:\projects\CLionProjects\myc1\thirdParty\sqlite3\sqlite3.c

CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.i"
	D:\Install\CodeBlocks\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\projects\CLionProjects\myc1\thirdParty\sqlite3\sqlite3.c > CMakeFiles\myc1.dir\thirdParty\sqlite3\sqlite3.c.i

CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.s"
	D:\Install\CodeBlocks\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\projects\CLionProjects\myc1\thirdParty\sqlite3\sqlite3.c -o CMakeFiles\myc1.dir\thirdParty\sqlite3\sqlite3.c.s

# Object files for target myc1
myc1_OBJECTS = \
"CMakeFiles/myc1.dir/dev-Tan/main.cpp.obj" \
"CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.obj"

# External object files for target myc1
myc1_EXTERNAL_OBJECTS =

myc1.exe: CMakeFiles/myc1.dir/dev-Tan/main.cpp.obj
myc1.exe: CMakeFiles/myc1.dir/thirdParty/sqlite3/sqlite3.c.obj
myc1.exe: CMakeFiles/myc1.dir/build.make
myc1.exe: CMakeFiles/myc1.dir/linklibs.rsp
myc1.exe: CMakeFiles/myc1.dir/objects1.rsp
myc1.exe: CMakeFiles/myc1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\projects\CLionProjects\myc1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable myc1.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\myc1.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myc1.dir/build: myc1.exe

.PHONY : CMakeFiles/myc1.dir/build

CMakeFiles/myc1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\myc1.dir\cmake_clean.cmake
.PHONY : CMakeFiles/myc1.dir/clean

CMakeFiles/myc1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\projects\CLionProjects\myc1 D:\projects\CLionProjects\myc1 D:\projects\CLionProjects\myc1\cmake-build-debug D:\projects\CLionProjects\myc1\cmake-build-debug D:\projects\CLionProjects\myc1\cmake-build-debug\CMakeFiles\myc1.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myc1.dir/depend

