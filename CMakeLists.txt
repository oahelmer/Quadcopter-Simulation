cmake_minimum_required(VERSION 3.10)
project(Simulation)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(NLopt REQUIRED)

# Print NLopt paths for debugging
message(STATUS "NLopt include dirs: ${NLopt_INCLUDE_DIRS}")
message(STATUS "NLopt libraries: ${NLopt_LIBRARIES}")

# Add executable
add_executable(Simulation
    main.cpp
    rigidBodyModule/rigidBody.cpp
    rigidBodyModule/nmpcController.cpp
)

# Include directories
target_include_directories(Simulation PRIVATE
    ${PROJECT_SOURCE_DIR}/rigidBodyModule
    ${NLopt_INCLUDE_DIRS}
    /usr/include/eigen3
)

# Link libraries
target_link_libraries(Simulation
    OpenGL::GL
    glfw
    GLEW
    GLU
    Threads::Threads
    ${NLopt_LIBRARIES}
    nlopt_cxx
)