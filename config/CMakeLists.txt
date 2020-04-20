set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 3.3)
project(config VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

add_library(config INTERFACE)
target_include_directories(config INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(config)
