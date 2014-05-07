if(GENERATE_DOCS)
  find_package(Doxygen REQUIRED)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_GEN_DIR}/doc/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_GEN_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${CMAKE_GEN_DIR}/doc
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  if(INSTALL_DOCS)
    # currently we install man files into DOCDIR, but we should move to MANDIR in the future
    install(DIRECTORY ${CMAKE_GEN_DIR}/doc/doc/man DESTINATION ${CMAKE_INSTALL_FULL_DOCDIR})
    install(DIRECTORY ${CMAKE_GEN_DIR}/doc/doc/html DESTINATION ${CMAKE_INSTALL_FULL_DOCDIR})
  endif()
endif()
