# main build script for all wiesel projects


# ensure a correct version of cmake is installed
cmake_minimum_required(VERSION 2.8)


# compatibility
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required


# check if WIESEL_DIR is set and exists
if(NOT WIESEL_DIR)
	message(FATAL_ERROR "variable WIESEL_DIR missing")
endif()

# Add module path
set(
	CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${WIESEL_DIR}/build/cmake/modules"
)

# paths
set(WIESEL_SRC_DIR       ${WIESEL_DIR}/src)
set(WIESEL_MODULES_DIR   ${WIESEL_DIR}/libraries/desktop)

# load standard modules
include(wiesel-module)


# load optional config includes
include(config/local/paths.cmake OPTIONAL)



# on win32 we may need to copy all SDL DLL's to our binary directory
if (WIN32)
	if (DEFINED EXTRALIBS_PATH)
		file(GLOB DLLS_TO_COPY ${EXTRALIBS_PATH}/bin/*.dll)
		file(COPY ${DLLS_TO_COPY} DESTINATION .)
	endif()
endif()
