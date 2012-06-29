PROJECT(TRIOSLIB)
SET(TRIOSLIB_INCLUDE_DIR ./include)
INCLUDE_DIRECTORIES(${TRIOSLIB_INCLUDE_DIR} )

SET(CMAKE_BUILD_TYPE debug)

SUBDIRS(include src test)


#-----------------------------------------------------------------------------
# TRIOS required special compiler flags
# TODO: Mostly, these settings were copied from the VTK config file
#       Must adapt to TRIOS needs
IF(CMAKE_COMPILER_IS_GNUCXX)
  # A GCC compiler
  # TODO: add required flags
  # SET(TRIOS_REQUIRED_CXX_FLAGS "${TRIOS_REQUIRED_CXX_FLAGS} <INSERT FLAGS HERE>")
  IF(WIN32)
    # The platform is gcc on cygwin.
    #SET(TRIOS_REQUIRED_CXX_FLAGS "${TRIOS_REQUIRED_CXX_FLAGS} -mwin32")
    #SET(TRIOS_REQUIRED_C_FLAGS "${TRIOS_REQUIRED_C_FLAGS} -mwin32")
  ENDIF(WIN32)
  IF(MINGW)
    #LINK_LIBRARIES(-lgdi32)
  ENDIF(MINGW)
  IF(CMAKE_SYSTEM MATCHES "SunOS.*")
  ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(CMAKE_ANSI_CFLAGS)
    SET(TRIOS_REQUIRED_C_FLAGS "${TRIOS_REQUIRED_C_FLAGS} ${CMAKE_ANSI_CFLAGS}")
  ENDIF(CMAKE_ANSI_CFLAGS)
  #silence duplicate symbol warnings on AIX
  IF(CMAKE_SYSTEM MATCHES "AIX.*")
    SET(TRIOS_REQUIRED_EXE_LINKER_FLAGS "${TRIOS_REQUIRED_EXE_LINKER_FLAGS} -bhalt:5")
  ENDIF(CMAKE_SYSTEM MATCHES "AIX.*")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(APPLE)
  SET(TRIOS_REQUIRED_C_FLAGS "${TRIOS_REQUIRED_C_FLAGS} -no-cpp-precomp")
  SET(TRIOS_REQUIRED_CXX_FLAGS "${TRIOS_REQUIRED_CXX_FLAGS} -no-cpp-precomp")
  IF(BUILD_SHARED_LIBS)
    SET(TRIOS_APPLE_SHARED_FLAGS_NEEDED 1)
  ENDIF(BUILD_SHARED_LIBS)
ENDIF(APPLE)

get_directory_property(TRIOS_BASIC_SRCS DIRECTORY src TRIOS_BASIC_SRCS)
message("SRC ${TRIOS_BASIC_SRCS}")


# Save the compiler settings so another project can import them.
#INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
#CMAKE_EXPORT_BUILD_SETTINGS(${TRIOS_BINARY_DIR}/TRIOSBuildSettings.cmake)
# TODO: update to use the new INSTALL command
# commented out because it broke the build; probably a directory
# mismatch
# INSTALL_FILES(/lib/pac .cmake TRIOSBuildSettings)



#-----------------------------------------------------------------------------
# Add compiler flags TRIOS needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TRIOS_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TRIOS_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TRIOS_REQUIRED_EXE_LINKER_FLAGS}")
