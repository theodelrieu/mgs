cmake_minimum_required(VERSION 3.3)

add_executable(test_base64 test_base64.cpp $<TARGET_OBJECTS:codec_tests_main>)
target_link_libraries(test_base64 base64 CONAN_PKG::catch2 CONAN_PKG::mgs_meta mgs_codec_helpers)

include(ExternalData)

set(ExternalData_URL_TEMPLATES
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base64/%(algo)/%(hash)"
)

ExternalData_Add_Test(base64_data NAME test_base64 COMMAND test_base64 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b64})
ExternalData_Add_Target(base64_data)

add_dependencies(test_base64 base64_data)
