# -----------------------------------------------------------------------------
# Find the DirectX SDK
#
# Defines
# DIRECTX_FOUND					- True, when DirectX was found
# DIRECTX_ROOT_DIR				- Root directory of the DirectX SDK
# DIRECTX_D3D<??>_FOUND			- True, when Direct3D in a specific version was found
# DIRECTX_D3D<??>_INCLUDE_DIR	- The include path for the d3d header in a specific version
# DIRECTX_D3D<??>_LIBRARY		- The library for Direct3D in a specific version

set(DIRECTX_FOUND FALSE)

# only makes sense on windows platform
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	if (CMAKE_CL_64)
		set (DIRECTX_ARCHITECTURE x64)
	else ()
		set (DIRECTX_ARCHITECTURE x86)
	endif ()

	# DirectX SDK
	find_path(
		DIRECTX_ROOT_DIR
		Include/d3d9.h
		PATHS
			"$ENV{DXSDK_DIR}"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (June 2010)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (June 2010)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (February 2010)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (February 2010)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (March 2009)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (March 2009)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (August 2008)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (August 2008)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (June 2008)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (June 2008)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (March 2008)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (March 2008)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (November 2007)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (November 2007)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (August 2007)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (August 2007)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK"
		DOC "DirectX SDK root directory"
	)
	
	if (DIRECTX_ROOT_DIR)
		set (DIRECTX_INC_SEARCH_PATH "${DIRECTX_ROOT_DIR}/Include")
		set (DIRECTX_LIB_SEARCH_PATH "${DIRECTX_ROOT_DIR}/Lib/${DIRECTX_ARCHITECTURE}")
		set (DIRECTX_BIN_SEARCH_PATH "${DIRECTX_ROOT_DIR}/Utilities/bin/x86")
	endif ()

	# With VS 2011 and Windows 8 SDK, the DirectX SDK is included as part of
	# the Windows SDK.
	#
	# See also:
	# - http://msdn.microsoft.com/en-us/library/windows/desktop/ee663275.aspx
	# TODO: Allow using DirectX SDK with VS 2011
	if (NOT ${CMAKE_SYSTEM_VERSION} LESS "6.2")
		find_path(
			WIN8_SDK_ROOT_DIR
			Include/um/windows.h
			PATHS
				"$ENV{ProgramFiles}/Windows Kits/8.0"
				"$ENV{ProgramFiles(x86)}/Windows Kits/8.0"
			DOC "Windows 8 SDK root directory"
		)

		if (WIN8_SDK_ROOT_DIR)
			set (DIRECTX_INC_SEARCH_PATH "${WIN8_SDK_ROOT_DIR}/Include/um" "${WIN8_SDK_ROOT_DIR}/Include/shared")
			set (DIRECTX_LIB_SEARCH_PATH "${WIN8_SDK_ROOT_DIR}/Lib/Win8/um/${DIRECTX_ARCHITECTURE}")
			set (DIRECTX_BIN_SEARCH_PATH "${WIN8_SDK_ROOT_DIR}/bin/${DIRECTX_ARCHITECTURE}")
		endif ()
	endif ()

	# set a list of possible DirectX versions
	set(DIRECTX_POSSIBLE_VERSIONS	8 9 10 10_1 11 11_1)
	
	foreach(version ${DIRECTX_POSSIBLE_VERSIONS})
		# find Direct3D header and library
		find_path(
					DIRECTX_D3D${version}_INCLUDE_DIR
					"d3d${version}.h"
					PATHS	${DIRECTX_INC_SEARCH_PATH}
					DOC		"The include path for the Direct3D ${version} header."
		)
		
		find_library(
					DIRECTX_D3D${version}_LIBRARY
					"d3d${version}"
					PATHS	${DIRECTX_LIB_SEARCH_PATH}
					DOC		"The library path for the Direct3D ${version} library."
		)
		
		if (DIRECTX_D3D${version}_INCLUDE_DIR AND DIRECTX_D3D${version}_LIBRARY)
			set(DIRECTX_D3D${version}_FOUND TRUE)
			
			# when successful, also search for the according Direct3DX headers and library
			find_path(
						DIRECTX_D3DX${version}_INCLUDE_DIR
						"d3dx${version}.h"
						PATHS	${DIRECTX_INC_SEARCH_PATH}
						DOC		"The include path for the Direct3DX ${version} header."
			)
			
			find_library(
						DIRECTX_D3DX${version}_LIBRARY
						"d3dx${version}"
						PATHS	${DIRECTX_LIB_SEARCH_PATH}
						DOC		"The library path for the Direct3DX ${version} library."
			)
				
			if (DIRECTX_D3DX${version}_INCLUDE_DIR AND DIRECTX_D3DX${version}_LIBRARY)
				set(DIRECTX_D3DX${version}_FOUND TRUE)
			endif ()
		endif ()
		
		mark_as_advanced (
				DIRECTX_D3D${version}_FOUND
				DIRECTX_D3D${version}_INCLUDE_DIR
				DIRECTX_D3D${version}_LIBRARY
				
				DIRECTX_D3DX${version}_FOUND
				DIRECTX_D3DX${version}_INCLUDE_DIR
				DIRECTX_D3DX${version}_LIBRARY
		)
	endforeach(version ${DIRECTX_POSSIBLE_VERSIONS})

	# search for d2d
	find_path(
		DIRECTX_D2D1_INCLUDE_DIR d2d1.h
		PATHS "${DirectX_ROOT_DIR}/Include"
		DOC "The directory where d2d1.h resides"
	)

	find_library(
		DIRECTX_D2D1_LIBRARY d2d1
		PATHS "${DirectX_ROOT_DIR}/Lib/x86"
		DOC "The directory where d2d1 resides"
	)

	if (DIRECTX_D2D1_INCLUDE_DIR AND DIRECTX_D2D1_LIBRARY)
		set (DIRECTX_D2D1_FOUND TRUE)
	endif(DIRECTX_D2D1_INCLUDE_DIR AND DIRECTX_D2D1_LIBRARY)

	mark_as_advanced(
			DIRECTX_D2D1_FOUND
			DIRECTX_D2D1_INCLUDE_DIR
			DIRECTX_D2D1_LIBRARY
	)

	# search for shader compiler executable
	find_program(
		DIRECTX_FXC_EXECUTABLE fxc
		PATHS ${DIRECTX_BIN_SEARCH_PATH}
		DOC "Path to fxc.exe executable."
	)

	mark_as_advanced(
			DIRECTX_ARCHITECTURE
			DIRECTX_ROOT_DIR
			DIRECTX_POSSIBLE_VERSIONS
			
			DIRECTX_INC_SEARCH_PATH
			DIRECTX_LIB_SEARCH_PATH
			DIRECTX_BIN_SEARCH_PATH
			
			DIRECTX_FXC_EXECUTABLE
	)
endif ()
