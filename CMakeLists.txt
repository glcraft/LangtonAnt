cmake_minimum_required(VERSION 3.12)

project(LangtonAnt)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML COMPONENTS window CONFIG REQUIRED)

file(GLOB PROJ_SRC
    "src/*.cpp"
    "include/*.h"
)

add_executable(LangtonAnt ${PROJ_SRC})

target_link_libraries(LangtonAnt 
    PRIVATE 
        GLEW::GLEW
        OpenGL
        sfml-window
)
target_include_directories(LangtonAnt PRIVATE "${CMAKE_SOURCE_DIR}/include")

set_target_properties(LangtonAnt PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)