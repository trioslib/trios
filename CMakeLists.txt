PROJECT(TRIOS)
cmake_minimum_required(VERSION 2.8)


INCLUDE_DIRECTORIES(trioslib/include)

# Output directories.
SET(LIBRARY_OUTPUT_PATH ${TRIOS_BINARY_DIR}/lib CACHE INTERNAL "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${TRIOS_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")

# Make sure the linker can find the TRIOS library once it is built.
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

# If set, runtime paths are not added when using shared libraries
SET(CMAKE_SKIP_RPATH ON)


#Enable tests
ENABLE_TESTING()

####### Doxygen #############
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docs
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

####### Package section ##########
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TRIOS - Training Image Operators from Samples")
SET(CPACK_PACKAGE_VENDOR "Computer Vision Lab, IME-USP")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_GENERATOR "TGZ")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
"TRIOS-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.
${PACK_PACKAGE_VERSION_PATCH}")
INCLUDE(CPack)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/README.txt" "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt"
    DESTINATION .)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/python/trios/" DESTINATION "python/trios")


####### subdirs ##########
SUBDIRS(trioslib)