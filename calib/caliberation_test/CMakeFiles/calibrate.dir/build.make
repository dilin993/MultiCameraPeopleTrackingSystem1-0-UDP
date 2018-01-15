# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test

# Include any dependencies generated for this target.
include CMakeFiles/calibrate.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/calibrate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/calibrate.dir/flags.make

CMakeFiles/calibrate.dir/src/main.cpp.o: CMakeFiles/calibrate.dir/flags.make
CMakeFiles/calibrate.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/calibrate.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/calibrate.dir/src/main.cpp.o -c /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test/src/main.cpp

CMakeFiles/calibrate.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/calibrate.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test/src/main.cpp > CMakeFiles/calibrate.dir/src/main.cpp.i

CMakeFiles/calibrate.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/calibrate.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test/src/main.cpp -o CMakeFiles/calibrate.dir/src/main.cpp.s

CMakeFiles/calibrate.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/calibrate.dir/src/main.cpp.o.requires

CMakeFiles/calibrate.dir/src/main.cpp.o.provides: CMakeFiles/calibrate.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/calibrate.dir/build.make CMakeFiles/calibrate.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/calibrate.dir/src/main.cpp.o.provides

CMakeFiles/calibrate.dir/src/main.cpp.o.provides.build: CMakeFiles/calibrate.dir/src/main.cpp.o


# Object files for target calibrate
calibrate_OBJECTS = \
"CMakeFiles/calibrate.dir/src/main.cpp.o"

# External object files for target calibrate
calibrate_EXTERNAL_OBJECTS =

calibrate: CMakeFiles/calibrate.dir/src/main.cpp.o
calibrate: CMakeFiles/calibrate.dir/build.make
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_face.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_text.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_video.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.3.2.0
calibrate: /usr/lib/x86_64-linux-gnu/libopencv_core.so.3.2.0
calibrate: CMakeFiles/calibrate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable calibrate"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/calibrate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/calibrate.dir/build: calibrate

.PHONY : CMakeFiles/calibrate.dir/build

CMakeFiles/calibrate.dir/requires: CMakeFiles/calibrate.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/calibrate.dir/requires

CMakeFiles/calibrate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/calibrate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/calibrate.dir/clean

CMakeFiles/calibrate.dir/depend:
	cd /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test /home/MultiCameraPeopleTrackingSystem1-0-UDP/caliberation_test/CMakeFiles/calibrate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/calibrate.dir/depend
