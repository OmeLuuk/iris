# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luuk/dev/git/iris

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luuk/dev/git/iris/build

# Include any dependencies generated for this target.
include CMakeFiles/iris.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/iris.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/iris.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/iris.dir/flags.make

CMakeFiles/iris.dir/src/main.cc.o: CMakeFiles/iris.dir/flags.make
CMakeFiles/iris.dir/src/main.cc.o: ../src/main.cc
CMakeFiles/iris.dir/src/main.cc.o: CMakeFiles/iris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luuk/dev/git/iris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/iris.dir/src/main.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/iris.dir/src/main.cc.o -MF CMakeFiles/iris.dir/src/main.cc.o.d -o CMakeFiles/iris.dir/src/main.cc.o -c /home/luuk/dev/git/iris/src/main.cc

CMakeFiles/iris.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/iris.dir/src/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luuk/dev/git/iris/src/main.cc > CMakeFiles/iris.dir/src/main.cc.i

CMakeFiles/iris.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/iris.dir/src/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luuk/dev/git/iris/src/main.cc -o CMakeFiles/iris.dir/src/main.cc.s

# Object files for target iris
iris_OBJECTS = \
"CMakeFiles/iris.dir/src/main.cc.o"

# External object files for target iris
iris_EXTERNAL_OBJECTS =

iris: CMakeFiles/iris.dir/src/main.cc.o
iris: CMakeFiles/iris.dir/build.make
iris: CMakeFiles/iris.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luuk/dev/git/iris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable iris"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/iris.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/iris.dir/build: iris
.PHONY : CMakeFiles/iris.dir/build

CMakeFiles/iris.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/iris.dir/cmake_clean.cmake
.PHONY : CMakeFiles/iris.dir/clean

CMakeFiles/iris.dir/depend:
	cd /home/luuk/dev/git/iris/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luuk/dev/git/iris /home/luuk/dev/git/iris /home/luuk/dev/git/iris/build /home/luuk/dev/git/iris/build /home/luuk/dev/git/iris/build/CMakeFiles/iris.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/iris.dir/depend

