INCLUDE(FindDoxygen)

IF(DOXYGEN_EXECUTABLE-NOTFOUND)
ELSE(DOXYGEN_EXECUTABLE-NOTFOUND)
    SET(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    # These are used in Doxyfile.in and SimbodyConfig.cmake.in.
    SET(SIMBODY_INSTALL_DOXYGENDIR
        "${CMAKE_INSTALL_DOCDIR}/api/${PROJECT_NAME}/html")
    SET(SIMBODY_DOXYGEN_TAGFILE_NAME "SimbodyDoxygenTagfile")

    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in 
          ${DOXY_CONFIG}
          @ONLY )

    ADD_CUSTOM_TARGET(doxygen ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}) 

    FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/html/")
    INSTALL(DIRECTORY "${PROJECT_BINARY_DIR}/html/"
            DESTINATION "${SIMBODY_INSTALL_DOXYGENDIR}"
            )
    # This is just a shortcut to the Doxygen index.html.
    INSTALL(FILES "SimbodyAPI.html" DESTINATION "${CMAKE_INSTALL_DOCDIR}")
ENDIF(DOXYGEN_EXECUTABLE-NOTFOUND)

