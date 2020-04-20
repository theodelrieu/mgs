cmake_minimum_required(VERSION 3.3)

add_executable(test_base32hex test_base32hex.cpp $<TARGET_OBJECTS:codec_tests_main>)
target_link_libraries(test_base32hex base32hex CONAN_PKG::mgs_meta CONAN_PKG::catch2 mgs_codec_helpers)

include(ExternalData)

set(ExternalData_URL_TEMPLATES
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base32hex/%(algo)/%(hash)"
)

ExternalData_Add_Test(base32hex_data NAME test_base32hex COMMAND test_base32hex --test-file DATA{data/random_data} --test-file DATA{data/random_data.b32hex})
ExternalData_Add_Target(base32hex_data)

add_dependencies(test_base32hex base32hex_data)
