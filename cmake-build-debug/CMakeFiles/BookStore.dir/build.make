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
CMAKE_SOURCE_DIR = /home/sword/swordProject/Bookstore

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sword/swordProject/Bookstore/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BookStore.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/BookStore.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/BookStore.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BookStore.dir/flags.make

CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o: CMakeFiles/BookStore.dir/flags.make
CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o: ../src/unrolled_linked_list.cpp
CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o: CMakeFiles/BookStore.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sword/swordProject/Bookstore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o -MF CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o.d -o CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o -c /home/sword/swordProject/Bookstore/src/unrolled_linked_list.cpp

CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sword/swordProject/Bookstore/src/unrolled_linked_list.cpp > CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.i

CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sword/swordProject/Bookstore/src/unrolled_linked_list.cpp -o CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.s

# Object files for target BookStore
BookStore_OBJECTS = \
"CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o"

# External object files for target BookStore
BookStore_EXTERNAL_OBJECTS =

BookStore: CMakeFiles/BookStore.dir/src/unrolled_linked_list.cpp.o
BookStore: CMakeFiles/BookStore.dir/build.make
BookStore: CMakeFiles/BookStore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sword/swordProject/Bookstore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable BookStore"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BookStore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BookStore.dir/build: BookStore
.PHONY : CMakeFiles/BookStore.dir/build

CMakeFiles/BookStore.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BookStore.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BookStore.dir/clean

CMakeFiles/BookStore.dir/depend:
	cd /home/sword/swordProject/Bookstore/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sword/swordProject/Bookstore /home/sword/swordProject/Bookstore /home/sword/swordProject/Bookstore/cmake-build-debug /home/sword/swordProject/Bookstore/cmake-build-debug /home/sword/swordProject/Bookstore/cmake-build-debug/CMakeFiles/BookStore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BookStore.dir/depend

