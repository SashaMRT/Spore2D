cmake_minimum_required(VERSION 3.16)
project(Spore2D VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Dépendances
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

# Dossier des headers
include_directories(include)

# Tests
include(CTest)
enable_testing()
add_test(NAME Spore2D COMMAND Spore2D --test)
set_tests_properties(Spore2D PROPERTIES TIMEOUT 30)

# Fichiers sources
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

target_link_libraries(Spore2D PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

# --- INJECTION AUTOMATIQUE DES INCLUDES ---
target_precompile_headers(Spore2D PRIVATE
    # Standards
    <iostream>
    <vector>
    <list>
    <string>
    <cmath>
    <algorithm>
    <memory>
    <cstdlib>
    <ctime>
    <iomanip>

    # SFML
    <SFML/Graphics.hpp>
    <SFML/System.hpp>
    <SFML/Window.hpp>
    <SFML/Audio.hpp>

    # Tes Headers (Ordre important !)
    "include/Core/Application.hpp"
    "include/Model/Entity.hpp"
    "include/Model/Grass.hpp"
    "include/Model/Sheep.hpp"
    "include/Model/Wolf.hpp"
    "include/Model/Simulation.hpp"
    "include/View/Hud.hpp"
    "include/View/Renderer.hpp"
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})