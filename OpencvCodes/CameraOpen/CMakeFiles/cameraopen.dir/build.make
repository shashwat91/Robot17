# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ishu/Robot17/OpencvCodes/CameraOpen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ishu/Robot17/OpencvCodes/CameraOpen

# Include any dependencies generated for this target.
include CMakeFiles/cameraopen.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cameraopen.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cameraopen.dir/flags.make

CMakeFiles/cameraopen.dir/cameraopen.cpp.o: CMakeFiles/cameraopen.dir/flags.make
CMakeFiles/cameraopen.dir/cameraopen.cpp.o: cameraopen.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ishu/Robot17/OpencvCodes/CameraOpen/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/cameraopen.dir/cameraopen.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cameraopen.dir/cameraopen.cpp.o -c /home/ishu/Robot17/OpencvCodes/CameraOpen/cameraopen.cpp

CMakeFiles/cameraopen.dir/cameraopen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cameraopen.dir/cameraopen.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ishu/Robot17/OpencvCodes/CameraOpen/cameraopen.cpp > CMakeFiles/cameraopen.dir/cameraopen.cpp.i

CMakeFiles/cameraopen.dir/cameraopen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cameraopen.dir/cameraopen.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ishu/Robot17/OpencvCodes/CameraOpen/cameraopen.cpp -o CMakeFiles/cameraopen.dir/cameraopen.cpp.s

CMakeFiles/cameraopen.dir/cameraopen.cpp.o.requires:
.PHONY : CMakeFiles/cameraopen.dir/cameraopen.cpp.o.requires

CMakeFiles/cameraopen.dir/cameraopen.cpp.o.provides: CMakeFiles/cameraopen.dir/cameraopen.cpp.o.requires
	$(MAKE) -f CMakeFiles/cameraopen.dir/build.make CMakeFiles/cameraopen.dir/cameraopen.cpp.o.provides.build
.PHONY : CMakeFiles/cameraopen.dir/cameraopen.cpp.o.provides

CMakeFiles/cameraopen.dir/cameraopen.cpp.o.provides.build: CMakeFiles/cameraopen.dir/cameraopen.cpp.o

# Object files for target cameraopen
cameraopen_OBJECTS = \
"CMakeFiles/cameraopen.dir/cameraopen.cpp.o"

# External object files for target cameraopen
cameraopen_EXTERNAL_OBJECTS =

cameraopen: CMakeFiles/cameraopen.dir/cameraopen.cpp.o
cameraopen: CMakeFiles/cameraopen.dir/build.make
cameraopen: /usr/local/lib/libopencv_videostab.so.2.4.13
cameraopen: /usr/local/lib/libopencv_video.so.2.4.13
cameraopen: /usr/local/lib/libopencv_ts.a
cameraopen: /usr/local/lib/libopencv_superres.so.2.4.13
cameraopen: /usr/local/lib/libopencv_stitching.so.2.4.13
cameraopen: /usr/local/lib/libopencv_photo.so.2.4.13
cameraopen: /usr/local/lib/libopencv_ocl.so.2.4.13
cameraopen: /usr/local/lib/libopencv_objdetect.so.2.4.13
cameraopen: /usr/local/lib/libopencv_nonfree.so.2.4.13
cameraopen: /usr/local/lib/libopencv_ml.so.2.4.13
cameraopen: /usr/local/lib/libopencv_legacy.so.2.4.13
cameraopen: /usr/local/lib/libopencv_imgproc.so.2.4.13
cameraopen: /usr/local/lib/libopencv_highgui.so.2.4.13
cameraopen: /usr/local/lib/libopencv_gpu.so.2.4.13
cameraopen: /usr/local/lib/libopencv_flann.so.2.4.13
cameraopen: /usr/local/lib/libopencv_features2d.so.2.4.13
cameraopen: /usr/local/lib/libopencv_core.so.2.4.13
cameraopen: /usr/local/lib/libopencv_contrib.so.2.4.13
cameraopen: /usr/local/lib/libopencv_calib3d.so.2.4.13
cameraopen: /usr/lib/x86_64-linux-gnu/libGLU.so
cameraopen: /usr/lib/x86_64-linux-gnu/libGL.so
cameraopen: /usr/lib/x86_64-linux-gnu/libSM.so
cameraopen: /usr/lib/x86_64-linux-gnu/libICE.so
cameraopen: /usr/lib/x86_64-linux-gnu/libX11.so
cameraopen: /usr/lib/x86_64-linux-gnu/libXext.so
cameraopen: /usr/local/lib/libopencv_nonfree.so.2.4.13
cameraopen: /usr/local/lib/libopencv_ocl.so.2.4.13
cameraopen: /usr/local/lib/libopencv_gpu.so.2.4.13
cameraopen: /usr/local/lib/libopencv_photo.so.2.4.13
cameraopen: /usr/local/lib/libopencv_objdetect.so.2.4.13
cameraopen: /usr/local/lib/libopencv_legacy.so.2.4.13
cameraopen: /usr/local/lib/libopencv_video.so.2.4.13
cameraopen: /usr/local/lib/libopencv_ml.so.2.4.13
cameraopen: /usr/local/lib/libopencv_calib3d.so.2.4.13
cameraopen: /usr/local/lib/libopencv_features2d.so.2.4.13
cameraopen: /usr/local/lib/libopencv_highgui.so.2.4.13
cameraopen: /usr/local/lib/libopencv_imgproc.so.2.4.13
cameraopen: /usr/local/lib/libopencv_flann.so.2.4.13
cameraopen: /usr/local/lib/libopencv_core.so.2.4.13
cameraopen: CMakeFiles/cameraopen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable cameraopen"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cameraopen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cameraopen.dir/build: cameraopen
.PHONY : CMakeFiles/cameraopen.dir/build

CMakeFiles/cameraopen.dir/requires: CMakeFiles/cameraopen.dir/cameraopen.cpp.o.requires
.PHONY : CMakeFiles/cameraopen.dir/requires

CMakeFiles/cameraopen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cameraopen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cameraopen.dir/clean

CMakeFiles/cameraopen.dir/depend:
	cd /home/ishu/Robot17/OpencvCodes/CameraOpen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ishu/Robot17/OpencvCodes/CameraOpen /home/ishu/Robot17/OpencvCodes/CameraOpen /home/ishu/Robot17/OpencvCodes/CameraOpen /home/ishu/Robot17/OpencvCodes/CameraOpen /home/ishu/Robot17/OpencvCodes/CameraOpen/CMakeFiles/cameraopen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cameraopen.dir/depend

