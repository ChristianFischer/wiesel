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
set(WIESEL_TESTS_DIR     ${WIESEL_DIR}/tests)
set(WIESEL_MODULES_DIR   ${WIESEL_DIR}/libraries/desktop)
set(WIESEL_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

# all source-files can include from the "generated" dir
include_directories(${WIESEL_GENERATED_DIR})


# load standard modules
include(wiesel-registry)
include(wiesel-module)
include(wiesel-testsuite)


# load optional config includes
include(config/local/paths.cmake OPTIONAL)



# enable warnings?
option(WIESEL_BUILD_WARN_ALL "Enable all warnings on build. (gcc option -Wall)" ON)
if (WIESEL_BUILD_WARN_ALL)
	set(CMAKE_CXX_FLAGS "-g -Wall")
	set(CMAKE_C_FLAGS "-g -Wall")
endif(WIESEL_BUILD_WARN_ALL)
