cmake_minimum_required(VERSION 3.14)

project(desunes)

option(BUILD_SHARED_LIBS "" OFF)

add_subdirectory(external)

add_subdirectory(application)
