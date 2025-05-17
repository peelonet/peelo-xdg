cmake_minimum_required(VERSION 3.12)

project(
  PeeloXdg
  VERSION 1.0.0
  DESCRIPTION "C++17 replacement for libxdg-basedir"
  HOMEPAGE_URL "https://github.com/peelonet/peelo-xdg"
  LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(
  ${PROJECT_NAME}
  INTERFACE
    cxx_std_17
)

install(
  TARGETS
    ${PROJECT_NAME}
  EXPORT
    ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)

write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION
    ${PROJECT_VERSION}
  COMPATIBILITY
    SameMajorVersion
)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
  EXPORT
    ${PROJECT_NAME}_Targets
  FILE
    ${PROJECT_NAME}Targets.cmake
  NAMESPACE
    ${PROJECT_NAME}::
  DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  install(
    DIRECTORY
      ${PROJECT_SOURCE_DIR}/include/peelo
    DESTINATION
      include
  )

  enable_testing()
  add_subdirectory(test)
endif()
