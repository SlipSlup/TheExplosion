
cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

set(PROJECT_NAME TheExplosion)

add_subdirectory(TheExplosionCore)
add_subdirectory(TheExplosionGame)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TheExplosionGame)
