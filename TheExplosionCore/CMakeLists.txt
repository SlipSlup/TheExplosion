
cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

set(CORE_NAME TheExplosionCore)

set(

	CORE_PUBLIC_INCLUDES
	
	includes/application.hpp
	includes/event.hpp
	includes/camera.hpp
	includes/keys.hpp
	includes/input.hpp
	includes/vertex_array.hpp
	includes/vertex_buffer.hpp
	includes/index_buffer.hpp
	
)

set(

	CORE_PRIVATE_INCLUDES
	
	src/window.hpp
	src/shader_program.hpp
	src/renderer.hpp
	src/UI.hpp
	src/texture2d.hpp
	
)

set(

	CORE_PRIVATE_SOURCES
	
	src/application.cpp
	src/window.cpp
	src/shader_program.cpp
	src/vertex_buffer.cpp
	src/vertex_array.cpp
	src/index_buffer.cpp
	src/camera.cpp
	src/renderer.cpp
	src/UI.cpp
	src/input.cpp
	src/texture2d.cpp
	
)

set(

	CORE_ALL_SOURCES
	
	${CORE_PRIVATE_SOURCES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PUBLIC_INCLUDES}
	
)

add_library(${CORE_NAME} STATIC ${CORE_ALL_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_ALL_SOURCES})

target_include_directories(${CORE_NAME} PUBLIC includes)
target_include_directories(${CORE_NAME} PRIVATE src)
target_compile_features(${CORE_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_NAME} PRIVATE glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${CORE_NAME} PRIVATE glm)

add_subdirectory(../external/stb_image ${CMAKE_CURRENT_BINARY_DIR}/stb_image)
target_link_libraries(${CORE_NAME} PRIVATE stb_image)

add_subdirectory(../external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${CORE_NAME} PRIVATE assimp)

set(

	IMGUI_INCLUDES
	
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
	
)

set(

	IMGUI_SOURCES
	
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_draw.cpp
	
)

add_library(

	ImGui STATIC
	
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
	
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${CORE_NAME} PRIVATE ImGui)
