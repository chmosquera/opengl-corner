# Locate the glfw3 library
#
# This module defines the following variables:
#
# GLEW_LIBRARY the name of the library;
# GLEW_INCLUDE_DIR where to find glfw include files.
# GLEW_FOUND true if both the GLEW_LIBRARY and GLEW_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you can define a
# variable called GLEW_ROOT which points to the root of the glfw library
# installation.
#
# default search dirs
# 
# Cmake file from: https://github.com/daw42/glslcookbook

set( _HEADER_SEARCH_DIRS
	"/usr/include"
	"/usr/local/include"
	"${CMAKE_SOURCE_DIR}/include"
	"C:/Program Files (x86)/glew/include" 
)

set( _LIB_SEARCH_DIRS
	"/usr/lib"
	"/usr/local/lib"
	"${CMAKE_SOURCE_DIR}/lib"
	# "C:/Program Files (x86)/glew/<library>"
)

# Check environment for root search directory
set( _ENV_ROOT $ENV{GLEW_ROOT} )
if( NOT GLEW_ROOT AND _ENV_ROOT )
	set(GLEW_ROOT ${_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( GLEW_ROOT )
	list( INSERT _HEADER_SEARCH_DIRS 0 "${GLEW_ROOT}/include" )
	list( INSERT _LIB_SEARCH_DIRS 0 "${GLEW_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(GLEW_INCLUDE_DIR "GL/glew.h"
PATHS ${_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(GLEW_LIBRARY NAMES GLEW
PATHS ${_LIB_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
GLEW_LIBRARY GLEW_INCLUDE_DIR)