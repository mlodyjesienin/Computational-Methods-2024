cmake_minimum_required(VERSION 3.22)
project(Binary_images)

set(CMAKE_CXX_STANDARD 17)

# Find Boost package
find_package(Boost REQUIRED COMPONENTS iostreams)

# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

# Add executable and link against Boost libraries
add_executable(Binary_images main.cpp
        energyTypes/energyfunctions.cpp
        energyTypes/energyfunctions.h
        utilities/plot_drawing.cpp
        utilities/plot_drawing.h
        utilities/gif_creation.cpp
        utilities/gif_creation.h
        algorithm/find_minimum_energy.cpp
        algorithm/find_minimum_energy.h
        energyTypes/neighbourhood_types.h
)

target_link_libraries(Binary_images ${Boost_LIBRARIES})

