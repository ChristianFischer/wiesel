# glee library


# create the library
wiesel_create_module(glee ${WIESEL_SRC_DIR}/third-party/glee-5.4 "")

# export include directories of this library for other targets
wiesel_module_export_includes(glee ${WIESEL_SRC_DIR}/third-party/glee-5.4)


# add opengl dependency
include(FindOpenGL)

if(DEFINED OPENGL_FOUND)
	wiesel_module_export_includes(glee ${WIESEL_SRC_DIR}/third-party/glee-5.4)
	wiesel_target_add_libraries(glee ${OPENGL_LIBRARIES})
	wiesel_target_add_includes(glee ${OPENGL_INCLUDE_DIR})
	
	# TODO: OpenGL and GLee includes are globally. Need to remove when no longer required
	include_directories(${OPENGL_INCLUDE_DIR})
	include_directories(${WIESEL_SRC_DIR}/third-party/glee-5.4)
else()
	message(FATAL_ERROR "required library opengl not found!")
endif()
