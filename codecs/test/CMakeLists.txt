cmake_minimum_required(VERSION 3.3)

add_library(codec_tests_main OBJECT catch_main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(codec_tests_main PRIVATE ${CATCH2_INC_DIR})

install(TARGETS codec_tests_main
  EXPORT codec_tests_main
  EXCLUDE_FROM_ALL
  OBJECTS
    DESTINATION bin
)

add_executable(test_codecs
  test_codecs.cpp
  test_codec_output.cpp
  test_codec_traits.cpp
  test_byte_type.cpp
  test_input_source.cpp
  test_sized_input_source.cpp
  test_basic_input_range_iterator.cpp
  test_iterator_sentinel_source.cpp
  test_codec.cpp
  $<TARGET_OBJECTS:codec_tests_main>
)

add_library(mgs_codec_helpers INTERFACE)
target_include_directories(mgs_codec_helpers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(test_codecs codecs CONAN_PKG::catch2 mgs_codec_helpers)

add_test(NAME test_codecs COMMAND test_codecs)
