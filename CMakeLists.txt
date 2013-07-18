PROJECT(TRIOS)
cmake_minimum_required(VERSION 2.8)

# Path for auxiliary CMake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)
INCLUDE_DIRECTORIES(trioslib/include)
# Output directories.
SET(LIBRARY_OUTPUT_PATH ${TRIOS_BINARY_DIR}/lib CACHE INTERNAL "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${TRIOS_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
# Make sure the linker can find the TRIOS library once it is built.
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
SET(BUILD_SHARED_LIBS ${TRIOS_BUILD_SHARED})
# If set, runtime paths are not added when using shared libraries
SET(CMAKE_SKIP_RPATH ON)

# Options
OPTION(TRIOS_BUILD_SHARED "Build a shared version of TRIOS libraries" OFF)
OPTION(WRAP_PYTHON "Build TRIOS wrapper for python." ON)
OPTION(BUILD_LATEX "Build latex (TRIOS guide) documentation." ON)
OPTION(BUILD_CXREF "Build cxref (API) documentation." ON)

#Enable tests
ENABLE_TESTING()

#Doxygen Automatic Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docs
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)


#Project subdirectories
SUBDIRS(trioslib)

####### CPack section ##########
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TRIOS library for designing image
    operators")
SET(CPACK_PACKAGE_VENDOR "Vision Laboratory, IME-USP")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
#SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
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

################################
