cmake_minimum_required(VERSION 3.10)
project(Simulation)

set(CMAKE_CXX_STANDARD 17)

# Find dependencies
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# Better NLopt detection using PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(NLOPT REQUIRED nlopt)

# Add executable
add_executable(Simulation
    main.cpp
    rigidBodyModule/rigidBody.cpp
    rigidBodyModule/nmpcController.cpp
)

target_include_directories(Simulation PRIVATE
    ${PROJECT_SOURCE_DIR}/rigidBodyModule
    ${NLOPT_INCLUDE_DIRS}
    /usr/include/eigen3
)

target_link_libraries(Simulation
    OpenGL::GL
    glfw
    GLEW
    GLU
    Threads::Threads
    ${NLOPT_LIBRARIES}
)