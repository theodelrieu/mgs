cmake_minimum_required(VERSION 3.3)

add_executable(test_base32 test_base32.cpp $<TARGET_OBJECTS:codec_tests_main>)
target_link_libraries(test_base32 base32 CONAN_PKG::mgs_meta CONAN_PKG::catch2 mgs_codec_helpers)

include(ExternalData)

set(ExternalData_URL_TEMPLATES
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base32/%(algo)/%(hash)"
)

ExternalData_Add_Test(base32_data NAME test_base32 COMMAND test_base32 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b32})
ExternalData_Add_Target(base32_data)

add_dependencies(test_base32 base32_data)
