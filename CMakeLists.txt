cmake_minimum_required(VERSION 3.10)
project(Spore2D VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

include_directories(include)
include(CTest)
enable_testing()

add_test(NAME Spore2D COMMAND Spore2D --test)
set_tests_properties(Spore2D PROPERTIES TIMEOUT 30)

set(SOURCES
    src/main.cpp
    src/Core/Application.cpp
    src/Model/Simulation.cpp
    src/Model/Entity.cpp
    src/Model/Wolf.cpp
    src/Model/Sheep.cpp
    src/Model/Grass.cpp
    src/View/Renderer.cpp
    src/View/Hud.cpp
)

add_executable(Spore2D ${SOURCES})

target_link_libraries(Spore2D PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)