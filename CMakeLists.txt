cmake_minimum_required(VERSION 3.14)

project(desunes)

option(BUILD_SHARED_LIBS "" OFF)

add_subdirectory(external/n_e_s)
add_subdirectory(external/imgui)
add_subdirectory(external/sfml)
add_subdirectory(external/imgui-sfml)

add_subdirectory(application)
