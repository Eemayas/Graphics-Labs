# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "E:\6th sem\Graphics"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "E:\6th sem\Graphics\build"

# Include any dependencies generated for this target.
include CMakeFiles/Graphics_Labs.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Graphics_Labs.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Graphics_Labs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Graphics_Labs.dir/flags.make

CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj: CMakeFiles/Graphics_Labs.dir/flags.make
CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj: CMakeFiles/Graphics_Labs.dir/includes_CXX.rsp
CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj: E:/6th\ sem/Graphics/srcs/Projects/square.cpp
CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj: CMakeFiles/Graphics_Labs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="E:\6th sem\Graphics\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj"
	C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj -MF CMakeFiles\Graphics_Labs.dir\srcs\Projects\square.cpp.obj.d -o CMakeFiles\Graphics_Labs.dir\srcs\Projects\square.cpp.obj -c "E:\6th sem\Graphics\srcs\Projects\square.cpp"

CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.i"
	C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "E:\6th sem\Graphics\srcs\Projects\square.cpp" > CMakeFiles\Graphics_Labs.dir\srcs\Projects\square.cpp.i

CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.s"
	C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "E:\6th sem\Graphics\srcs\Projects\square.cpp" -o CMakeFiles\Graphics_Labs.dir\srcs\Projects\square.cpp.s

# Object files for target Graphics_Labs
Graphics_Labs_OBJECTS = \
"CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj"

# External object files for target Graphics_Labs
Graphics_Labs_EXTERNAL_OBJECTS =

Graphics_Labs.exe: CMakeFiles/Graphics_Labs.dir/srcs/Projects/square.cpp.obj
Graphics_Labs.exe: CMakeFiles/Graphics_Labs.dir/build.make
Graphics_Labs.exe: lib/glfw-3.4/src/libglfw3.a
Graphics_Labs.exe: lib/glad/libglad.a
Graphics_Labs.exe: CMakeFiles/Graphics_Labs.dir/linkLibs.rsp
Graphics_Labs.exe: CMakeFiles/Graphics_Labs.dir/objects1.rsp
Graphics_Labs.exe: CMakeFiles/Graphics_Labs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="E:\6th sem\Graphics\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Graphics_Labs.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Graphics_Labs.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Graphics_Labs.dir/build: Graphics_Labs.exe
.PHONY : CMakeFiles/Graphics_Labs.dir/build

CMakeFiles/Graphics_Labs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Graphics_Labs.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Graphics_Labs.dir/clean

CMakeFiles/Graphics_Labs.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "E:\6th sem\Graphics" "E:\6th sem\Graphics" "E:\6th sem\Graphics\build" "E:\6th sem\Graphics\build" "E:\6th sem\Graphics\build\CMakeFiles\Graphics_Labs.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/Graphics_Labs.dir/depend

