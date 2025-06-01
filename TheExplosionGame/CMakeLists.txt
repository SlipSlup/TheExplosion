
cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

set(GAME_NAME TheExplosionGame)

add_executable(${GAME_NAME} src/main.cpp)

target_link_libraries(${GAME_NAME} TheExplosionCore ImGui glm)
target_compile_features(${GAME_NAME} PUBLIC cxx_std_17)

set_target_properties(${GAME_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
