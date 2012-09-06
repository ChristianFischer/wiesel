
function(wiesel_module_add_dependency parent_target module)
#	message("load module ${parent_target} - ${module}")
	
	if(ARGC GREATER 2)
		set(MODULE_PATH ${WIESEL_MODULES_DIR}/${ARGN})
	else()
		set(MODULE_PATH ${WIESEL_MODULES_DIR}/${module})
	endif()
	
	if((NOT TARGET ${module}) AND (EXISTS ${MODULE_PATH}/module.cmake))
		include(${MODULE_PATH}/module.cmake)
	endif()
	
	# add dependency
	add_dependencies(${parent_target} ${module})
	target_link_libraries(${parent_target} ${module})

	# configure the include path for the parent target
	get_target_property(MODULE_INCLUDE_DIRECTORIES ${module} EXPORT_INCLUDE_DIRECTORIES)
	if (NOT MODULE_INCLUDE_DIRECTORIES STREQUAL "MODULE_INCLUDE_DIRECTORIES-NOTFOUND")
		wiesel_target_add_includes(${parent_target} ${MODULE_INCLUDE_DIRECTORIES})
	endif()

	# also add exported libraries to the parent target
#	get_target_property(MODULE_LIBRARIES ${module} EXPORT_LIBRARIES)
#	if (NOT MODULE_LIBRARIES STREQUAL "MODULE_LIBRARIES-NOTFOUND")
#		wiesel_target_add_libraries(${parent_target} ${MODULE_LIBRARIES})
#	endif()
	
#	get_target_property(INCLUDES_2 ${parent_target} INCLUDE_DIRECTORIES)
#	message("includes for ${parent_target}:  ${INCLUDES_2}")
	
endfunction(wiesel_module_add_dependency)



function(wiesel_module_export_includes module)
	set_property(TARGET ${module} APPEND PROPERTY EXPORT_INCLUDE_DIRECTORIES ${ARGN})
endfunction(wiesel_module_export_includes)



function(wiesel_module_export_libraries module)
	set_property(TARGET ${module} APPEND PROPERTY EXPORT_LIBRARIES ${ARGN})
endfunction(wiesel_module_export_libraries)



function(wiesel_module_get_files var path)

	file(GLOB_RECURSE MY_FILES ${path})
	set(${var} ${MY_FILES} PARENT_SCOPE)
	
endfunction(wiesel_module_get_files)



function(wiesel_create_executable target source_dir filter)
	# get all sources from this directory
	wiesel_module_get_files(MODULE_SRC_FILES ${source_dir}/${filter})
	
	# add sources to the module's target
	add_executable(${target} ${MODULE_SRC_FILES})
	
	# add the source dir as include directory to the target
	wiesel_target_add_includes(${target} ${source_dir})
	
endfunction(wiesel_create_executable)



function(wiesel_create_module target source_dir filter)
	# get all sources from this directory
	wiesel_module_get_files(MODULE_SRC_FILES ${source_dir}/${filter})
	
	# add sources to the module's target
	add_library(${target} SHARED ${MODULE_SRC_FILES})
	
	# add the source dir as include directory to the target
	wiesel_target_add_includes(${target} ${source_dir})
	
endfunction(wiesel_create_module)



function(wiesel_target_add_includes target)
	set_property(TARGET ${target} APPEND PROPERTY INCLUDE_DIRECTORIES ${ARGN})
	
	# modify compiler flags
	foreach(incl ${ARGN})
		wiesel_target_add_compileflags(${target} -I${incl})
	endforeach()
	
endfunction(wiesel_target_add_includes)



function(wiesel_target_add_libraries target)
	target_link_libraries(${target} ${ARGN})
endfunction(wiesel_target_add_libraries)



function(wiesel_target_add_compileflags target)
	get_property(TARGET_COMPILE_FLAGS TARGET ${target} PROPERTY COMPILE_FLAGS)
	
	foreach(flag ${ARGN})
		set(TARGET_COMPILE_FLAGS "${TARGET_COMPILE_FLAGS} ${flag}")
	endforeach()
	
	set_property(TARGET ${target} PROPERTY COMPILE_FLAGS ${TARGET_COMPILE_FLAGS})
endfunction(wiesel_target_add_compileflags)
