include_directories(${CMAKE_CURRENT_SOURCE_DIR}../include)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(TEST_FILENAME ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_FILENAME} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_FILENAME})

  target_include_directories(
    ${TEST_NAME}
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/../include
  )

  target_compile_features(
    ${TEST_NAME}
    PUBLIC
      cxx_std_17
  )

  target_link_libraries(
    ${TEST_NAME}
    PeeloXdg
  )

  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
