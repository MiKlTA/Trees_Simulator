cmake_minimum_required(VERSION 3.5)

project(trees_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(trees_simulator
    main.cpp
    
    opengl.h
    
    window.h
    window.cpp
    rect.h
    rect.cpp
    
    simulation_core.h
    simulation_core.cpp
    
    field.h
    field.cpp
    tile.h
    tree.h
    tree.cpp
    )

include_directories(D:/mingw32_810/LIBS/opencl/source/external/OpenCL-Headers)
include_directories(D:/mingw32_810/LIBS/opencl/source/external/OpenCL-CLHPP/include)
target_link_libraries(
    trees_simulator
    D:/mingw32_810/LIBS/opencl/lib/libOpenCL.a
    )
target_link_libraries(trees_simulator Cfgmgr32.lib)

include_directories(D:/mingw32_810/LIBS/glew/include)
target_link_libraries(
    trees_simulator
    D:/mingw32_810/LIBS/glew/lib/glew32s.lib
    )
target_link_libraries(trees_simulator opengl32.a)

include_directories(D:/mingw32_810/LIBS/glfw/source/include)
target_link_libraries(trees_simulator D:/mingw32_810/LIBS/glfw/lib/libglfw3.a)

include_directories(D:/mingw32_810/LIBS/glm)
target_link_libraries(
    trees_simulator
    D:/mingw32_810/LIBS/glm/lib/libglm_static.a
    )
