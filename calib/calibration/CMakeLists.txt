cmake_minimum_required(VERSION 3.5)
project(calibrate)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})


file(GLOB SOURCE_FILES
        "src/*.h"
        "src/*.cpp"
        "src/*.hpp"
        )


add_executable(calibrate ${SOURCE_FILES})

target_link_libraries( calibrate ${OpenCV_LIBS})
