find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(Doxygen.in Doxygen)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxygen"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxygen"
  )
endif(DOXYGEN_FOUND)
