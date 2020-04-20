macro(mgs_export_and_install_target _project_name)
  set(_targets_file_name "${_project_name}-targets")
  install(TARGETS ${_project_name} EXPORT ${_targets_file_name}
    COMPONENT ${_project_name}
  )

  install(DIRECTORY include DESTINATION . COMPONENT ${_project_name})

  include(CMakePackageConfigHelpers)

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-config-version.cmake"
    # Will change to SameMajorVersion once 1.0 is released.
    COMPATIBILITY SameMinorVersion
  )

  set(ConfigPackageLocation lib/cmake/mgs/${_project_name})
  set(MGS_TARGETS_FILEPATH "${ConfigPackageLocation}/${_targets_file_name}")

  find_file(_template_config_file MgsPackageConfig.cmake.in HINTS ${CMAKE_MODULE_PATH})
  configure_package_config_file(${_template_config_file}
    "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-config.cmake"
    INSTALL_DESTINATION ConfigPackageLocation
    PATH_VARS MGS_TARGETS_FILEPATH
  )

  export(EXPORT ${_targets_file_name}
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${target_filename}.cmake"
    NAMESPACE mgs::
  )

  install(EXPORT ${_targets_file_name}
    FILE
      "${_targets_file_name}.cmake"
    NAMESPACE
      mgs::
    DESTINATION
      ${ConfigPackageLocation}
  )

  install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-config-version.cmake"
    DESTINATION
      ${ConfigPackageLocation}
    COMPONENT
      ${_project_name}
  )

  # Always install mgs-config-version.cmake
  find_file(_mgs_config_file mgs-config.cmake HINTS ${CMAKE_MODULE_PATH})
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mgs-config-version.cmake"
    # Will change to SameMajorVersion once 1.0 is released.
    COMPATIBILITY SameMinorVersion
  )
  install(
    FILES
      ${_mgs_config_file}
      "${CMAKE_CURRENT_BINARY_DIR}/mgs-config-version.cmake"
    DESTINATION
      lib/cmake/mgs/
  )

  unset(ConfigPackageLocation)
  unset(MGS_TARGETS_FILEPATH)
  unset(_targets_file_name)
  unset(_template_file)
endmacro()
