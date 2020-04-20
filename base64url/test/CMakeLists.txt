cmake_minimum_required(VERSION 3.3)

add_executable(test_base64url test_base64url.cpp $<TARGET_OBJECTS:codec_tests_main>)
target_link_libraries(test_base64url base64url CONAN_PKG::catch2 CONAN_PKG::mgs_meta mgs_codec_helpers)

include(ExternalData)

set(ExternalData_URL_TEMPLATES
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base64url/%(algo)/%(hash)"
)

ExternalData_Add_Test(base64url_data NAME test_base64url COMMAND test_base64url --test-file DATA{data/random_data} --test-file DATA{data/random_data.b64url})
ExternalData_Add_Target(base64url_data)

add_dependencies(test_base64url base64url_data)
