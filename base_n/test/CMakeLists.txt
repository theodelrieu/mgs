cmake_minimum_required(VERSION 3.3)

add_executable(test_base_n test_base_n.cpp $<TARGET_OBJECTS:codec_tests_main>)
target_link_libraries(test_base_n base_n CONAN_PKG::catch2 CONAN_PKG::mgs_meta mgs_codec_helpers)

add_test(NAME test_base_n COMMAND test_base_n)
