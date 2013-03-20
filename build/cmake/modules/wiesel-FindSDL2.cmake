# -----------------------------------------------------------------------------
# include the SDL library, version 2 into a project
# like the cmake FindSDL module, but offers the ability
# to download and compile the library
#
# Defines
# SDL2_FOUND		- true when SDL was found
# SDL2_DEPENDENCY	- a dependency to add via add_dependencies, if defined
# SDL2_INCLUDE_DIR	- include directory for the compiler target
# SDL2_LIBRARY		- library to add to the compiler target


# When enabled, the SDL library will be downloaded and compiled
# otherwise, we try to find libsdl in the system 
option(
		WIESEL_BUILD_SDL2
		"Download and compile the SDL2 library from source instead of searching in the system for it."
		OFF
)


if (WIESEL_BUILD_SDL2)
	# required for FindHg, which was added with 2.8.10
	#cmake_minimum_required(VERSION 2.8.10)

	include(ExternalProject)
	find_package(Hg)

	# NOTE: no MD5 hash provided, since the download archive is a snapshot of 
	# the repository, which will chance very oftern
	set(WIESEL_BUILD_SDL2_DOWNLOAD_URL		"http://www.libsdl.org/tmp/SDL-2.0.zip")
	#set(WIESEL_BUILD_SDL2_DOWNLOAD_MD5		"")	
	set(WIESEL_BUILD_SDL2_HG_CHECKOUT_URL	"http://hg.libsdl.org/SDL")
	set(WIESEL_BUILD_SDL2_HG_CHECKOUT_TAG	"tip")

	set(
		WIESEL_BUILD_SDL2_CMAKE_ARGS
		-DSDL_SHARED=ON
		-DSDL_STATIC=OFF
	)

	# forward build type
	if (DEFINED CMAKE_BUILD_TYPE)
		set(
			WIESEL_BUILD_SDL2_CMAKE_ARGS
			${WIESEL_BUILD_SDL2_CMAKE_ARGS}
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		)
	endif(DEFINED CMAKE_BUILD_TYPE)

	if (NOT DEFINED ENV{DXSDK_DIR})
		# disabling DirectX for SDL, since no SDK seems to be installed 
		set(
			WIESEL_BUILD_SDL2_CMAKE_ARGS
			${WIESEL_BUILD_SDL2_CMAKE_ARGS}
			-DDIRECTX=OFF
		)
	endif(NOT DEFINED ENV{DXSDK_DIR})

	if (HG_FOUND)
		ExternalProject_Add(
				libSDL2
				PREFIX				libSDL2
				HG_REPOSITORY		${WIESEL_BUILD_SDL2_HG_CHECKOUT_URL}
				HG_TAG				${WIESEL_BUILD_SDL2_HG_CHECKOUT_TAG}

				# cmake config
				CMAKE_GENERATOR		"${CMAKE_GENERATOR}"
				CMAKE_ARGS			${WIESEL_BUILD_SDL2_CMAKE_ARGS}
				
				# disable update
				UPDATE_COMMAND		""

				# skip installation
				INSTALL_COMMAND 	""
		)
	else(HG_FOUND)
		ExternalProject_Add(
				libSDL2
				PREFIX				libSDL2
				URL					${WIESEL_BUILD_SDL2_DOWNLOAD_URL}

				# cmake config
				CMAKE_GENERATOR		"${CMAKE_GENERATOR}"
				CMAKE_ARGS			${WIESEL_BUILD_SDL2_CMAKE_ARGS}
				
				# disable update
				UPDATE_COMMAND		""

				# skip installation
				INSTALL_COMMAND 	""
		)
	endif(HG_FOUND)

	if (TARGET libSDL2)
		# get SDL directories
		ExternalProject_Get_Property(libSDL2 SOURCE_DIR)
		ExternalProject_Get_Property(libSDL2 BINARY_DIR)

		set(SDL2_LIBRARY_NAME	${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX})

		# set SDL include & library varialbles for usage in the application script
		set(SDL2_FOUND				TRUE												)
		set(SDL2_DEPENDENCY			libSDL2												)
		set(SDL2_INCLUDE_DIR		${SOURCE_DIR}/include								)
		set(SDL2_LIBRARY			${CMAKE_CURRENT_BINARY_DIR}/${SDL2_LIBRARY_NAME}	)

		# copy the created library into our build directory
		add_custom_command(
					TARGET			libSDL2		POST_BUILD
					COMMAND			${CMAKE_COMMAND} -E copy_if_different ${BINARY_DIR}/${SDL2_LIBRARY_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${SDL2_LIBRARY_NAME}
					MAIN_DEPENDENCY	${BINARY_DIR}/${SDL2_LIBRARY_NAME}
		)

	else(TARGET libSDL2)
		message(
				FATAL_ERROR 
				"Error at downloading the libSDL2 source."
		)
	endif(TARGET libSDL2)

else(WIESEL_BUILD_SDL2)
	message(
			FATAL_ERROR
			"required library SDL2 not found! Try using option WIESEL_BUILD_SDL2"
	)
endif(WIESEL_BUILD_SDL2)
