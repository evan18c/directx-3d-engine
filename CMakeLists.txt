cmake_minimum_required(VERSION 3.20)
project(Engine)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress warnings
add_compile_options(-w)

# Include directories
include_directories(
    include
    include/Engine/External/stb
    include/Engine/External/imgui
    include/Engine/External/imgui/backends
)

# Source files (recursively)
file(GLOB_RECURSE SOURCES
    "source/*.cpp"
    "source/Engine/Engine.cpp"
    "source/Engine/Core/*.cpp"
    "source/Engine/Game/*.cpp"
    "source/Engine/External/imgui/*.cpp"
    "source/Engine/Graphics/*.cpp"
    "source/Engine/Utils/*.cpp"
)

# Create the executable
add_executable(main ${SOURCES})

# Link libraries
target_link_libraries(main d3d11 d3dcompiler dxguid dxgi user32 gdi32 dwmapi)
