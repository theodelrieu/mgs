cmake_minimum_required(VERSION 3.3)

add_executable(test_base16 test_base16.cpp $<TARGET_OBJECTS:codec_tests_main>)
target_link_libraries(test_base16 base16 CONAN_PKG::mgs_meta CONAN_PKG::catch2 mgs_codec_helpers)

include(ExternalData)

set(ExternalData_URL_TEMPLATES
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base16/%(algo)/%(hash)"
)

ExternalData_Add_Test(base16_data NAME test_base16 COMMAND test_base16 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b16})
ExternalData_Add_Target(base16_data)

add_dependencies(test_base16 base16_data)
