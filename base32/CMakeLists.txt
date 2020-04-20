set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 3.3)
project(base32 VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

add_library(base32 INTERFACE)
target_include_directories(base32 INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(base32 INTERFACE CONAN_PKG::mgs_base_n CONAN_PKG::mgs_codecs CONAN_PKG::mgs_config)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(base32)
