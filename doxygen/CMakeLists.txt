find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(Doxyfile.in Doxyfile)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  )
endif(DOXYGEN_FOUND)
