cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 23)
project(HoverGame)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.2.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib_cpp
            GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
            GIT_TAG v4.2.6
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# This is the main part:
set(CUSTOM_INCLUDES ./includes)
include_directories(${CUSTOM_INCLUDES})

set(SOURCES main.cpp src/Player.cpp src/Player.h src/resources/Paths.cpp src/resources/Paths.h includes/Includes.h src/resources/Paths.cpp src/Player.cpp src/TerrainSegment.cpp src/TerrainSegment.h src/Collision.cpp src/Collision.h src/PerlinNoise.h src/PerlinNoise.cpp src/CityScape.cpp src/CityScape.h src/Particle.cpp src/TerrainEditor.cpp src/TerrainEditor.h src/TerrainEditor.h src/Random.cpp src/Random.h src/Util.cpp src/Util.h src/RagDoll.cpp src/RagDoll.h src/RigidBody.cpp src/RigidBody.h src/TerrainSerializer.cpp src/TerrainSerializer.h src/Game.cpp src/Game.h)
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raylib_cpp)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# That's it! You should have an example executable that you can run. Have fun!
