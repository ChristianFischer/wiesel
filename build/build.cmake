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


# try to add a symlink to the resources folder into the current build dir
option(WIESEL_BUILD_ADD_RESOURCES_SYMLINK "Tries to add a smbolic link to the project's resources folder into the binary directory" ON)
if (WIESEL_BUILD_ADD_RESOURCES_SYMLINK)
	if (WIN32)
	# the nice windows symlink does not work yet
	# it seems, normal users needs a special permissions to create links
	# but usually our build script won't run with administrator privileges
	#	if (NOT ${CMAKE_SYSTEM_VERSION} LESS "6.0")
	#		# windows >= vista supports symlinks
	#		add_custom_target(
	#					copy-resources
	#					COMMAND			cmd /S /C \"mklink /D 
	#										\"${CMAKE_CURRENT_BINARY_DIR}/resources\"
	#										\"${CMAKE_CURRENT_SOURCE_DIR}/resources\"
	#									\"
	#		)
	#	else()
			# no symlinks - just copy
			add_custom_target(
						copy-resources
						COMMAND			${CMAKE_COMMAND} -E copy_directory 
											"${CMAKE_CURRENT_SOURCE_DIR}/resources"
											"${CMAKE_CURRENT_BINARY_DIR}/resources"
			)
	#	endif()
	elseif(UNIX)
		add_custom_target(
					copy-resources
					COMMAND			${CMAKE_COMMAND} -E create_symlink 
										"${CMAKE_CURRENT_SOURCE_DIR}/resources"
										"${CMAKE_CURRENT_BINARY_DIR}/resources"
		)
	endif(WIN32)
endif(WIESEL_BUILD_ADD_RESOURCES_SYMLINK)
