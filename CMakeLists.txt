cmake_minimum_required(VERSION 3.3)
project(mgs CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if (NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include(CTest)

if (BUILD_TESTING)
  enable_testing()
endif()

include(${CMAKE_CURRENT_BINARY_DIR}/conanworkspace.cmake)
conan_workspace_subdirectories()
