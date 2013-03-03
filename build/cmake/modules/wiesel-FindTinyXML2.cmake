# -----------------------------------------------------------------------------
# include the TinyXML2 library into a project and offers the ability
# to download and compile the library
#
# Defines
# TINYXML2_FOUND		- true when tinyml was found
# TINYXML2_DEPENDENCY	- a dependency to add via add_dependencies, if defined
# TINYXML2_INCLUDE_DIR	- include directory for the compiler target
# TINYXML2_LIBRARY		- library to add to the compiler target


# When enabled, the TinyXML2 library will be downloaded and compiled
# otherwise, we try to find libTinyXML2 in the system 
option(
		WIESEL_BUILD_TINYXML
		"Download and compile the TinyXML2 library from source instead of searching in the system for it."
		OFF
)


if (WIESEL_BUILD_TINYXML2)
	include(ExternalProject)
	find_package(Git)

	#set(WIESEL_BUILD_TINYXML2_DOWNLOAD_URL		"")
	#set(WIESEL_BUILD_TINYXML2_DOWNLOAD_MD5		"")
	set(WIESEL_BUILD_TINYXML2_GIT_CHECKOUT_URL	"https://github.com/leethomason/tinyxml2.git")
	set(WIESEL_BUILD_TINYXML2_GIT_CHECKOUT_TAG	"HEAD")

	set(
		WIESEL_BUILD_TINYXML2_CMAKE_ARGS
	)

	# forward build type
	if (DEFINED CMAKE_BUILD_TYPE)
		set(
			WIESEL_BUILD_TINYXML2_CMAKE_ARGS
			${WIESEL_BUILD_TINYXML2_CMAKE_ARGS}
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		)
	endif(DEFINED CMAKE_BUILD_TYPE)

	if (GIT_FOUND)
		ExternalProject_Add(
				libTinyXML2
				PREFIX				libTinyXML2
				GIT_REPOSITORY		${WIESEL_BUILD_TINYXML2_GIT_CHECKOUT_URL}
				GIT_TAG				${WIESEL_BUILD_TINYXML2_GIT_CHECKOUT_TAG}

				# cmake config
				CMAKE_GENERATOR		"${CMAKE_GENERATOR}"
				CMAKE_ARGS			${WIESEL_BUILD_TINYXML2_CMAKE_ARGS}

				# skip installation
				INSTALL_COMMAND 	""
		)

		# get build directories
		ExternalProject_Get_Property(libTinyXML2 SOURCE_DIR)
		ExternalProject_Get_Property(libTinyXML2 BINARY_DIR)

		set(TINYXML2_LIBRARY_NAME	${CMAKE_SHARED_LIBRARY_PREFIX}tinyxml2${CMAKE_SHARED_LIBRARY_SUFFIX})

		# set TinyXML2 include & library varialbles for usage in the application script
		set(TINYXML2_FOUND			TRUE													)
		set(TINYXML2_DEPENDENCY		libTinyXML2												)
		set(TINYXML2_INCLUDE_DIR	${SOURCE_DIR}											)
		set(TINYXML2_LIBRARIES		${CMAKE_CURRENT_BINARY_DIR}/${TINYXML2_LIBRARY_NAME}	)

		# copy the created library into our build directory
		add_custom_command(
					TARGET			libTinyXML2		POST_BUILD
					COMMAND			${CMAKE_COMMAND} -E copy_if_different ${BINARY_DIR}/${TINYXML2_LIBRARY_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${TINYXML2_LIBRARY_NAME}
					MAIN_DEPENDENCY	${BINARY_DIR}/${TINYXML2_LIBRARY_NAME}
		)

	else(GIT_FOUND)
		message(
				FATAL_ERROR 
				"Missing git executable, which is required for downloading the TinyXML2 source."
		)
	endif(GIT_FOUND)

else(WIESEL_BUILD_TINYXML2)
	set(TINYXML2_FOUND			FALSE)
endif(WIESEL_BUILD_TINYXML2)
