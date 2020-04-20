cmake_minimum_required(VERSION 3.3)

function(create_meta_tests)
  set(filenames "")
  foreach (name ${ARGN})
    list(APPEND filenames test_${name}.cpp)
  endforeach()

  add_executable(test_meta
    ${filenames}
    main.cpp
  )
  target_link_libraries(test_meta meta CONAN_PKG::catch2)

  add_test(NAME test_meta COMMAND test_meta)
endfunction()

create_meta_tests(
  bidirectional_iterator
  boolean
  common_reference
  complete_type
  convertible_to
  constructible
  dereferenceable
  derived_from
  detected
  equality_comparable
  forward_iterator
  incrementable
  incrementable_traits
  input_iterator
  iter_concept
  iter_traits
  input_or_output_iterator
  iterator_traits
  output_iterator
  random_access_iterator
  range
  readable
  readable_traits
  regular
  semiregular
  sentinel_for
  sized_sentinel_for
  totally_ordered
  swappable
  writable
)
