
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
	
	# the parent target inherits all exported loaders from it's dependency
	get_target_property(MODULE_LOADERS ${module} EXPORT_MODULE_LOADERS)
	if (NOT MODULE_LOADERS STREQUAL "MODULE_LOADERS-NOTFOUND")
		wiesel_module_export_loaders(${parent_target} ${MODULE_LOADERS})
		
		# on executable targets, we need to append the module's registry
		set(MODULE_REGISTRY_FILE ${CMAKE_CURRENT_BINARY_DIR}/generated/${parent_target}-module-registry.cpp)
		wiesel_append_module_registry(${parent_target} ${MODULE_REGISTRY_FILE} ${MODULE_LOADERS})
	endif()
	
endfunction(wiesel_module_add_dependency)



function(wiesel_module_export_includes module)
	set_property(TARGET ${module} APPEND PROPERTY EXPORT_INCLUDE_DIRECTORIES ${ARGN})
endfunction(wiesel_module_export_includes)



function(wiesel_module_export_libraries module)
	set_property(TARGET ${module} APPEND PROPERTY EXPORT_LIBRARIES ${ARGN})
endfunction(wiesel_module_export_libraries)



function(wiesel_module_get_files var path)
	file(GLOB_RECURSE MY_FILES ${path}/*.c ${path}/*.cpp)
	set(${var} ${MY_FILES} PARENT_SCOPE)
endfunction(wiesel_module_get_files)



function(wiesel_create_executable target source_dir)
	set(MODULE_REGISTRY_FILE ${CMAKE_CURRENT_BINARY_DIR}/generated/${target}-module-registry.cpp)

	# get all sources from this directory
	wiesel_module_get_files(MODULE_SRC_FILES ${source_dir})
	
	# add sources to the module's target
	add_executable(${target} ${MODULE_REGISTRY_FILE} ${MODULE_SRC_FILES})
	
	# scan for all loadable modules in the library's source-tree
	wiesel_module_scan_for_loaders(MODULE_LOADERS ${source_dir})
	wiesel_module_export_loaders(${target} ${MODULE_LOADERS})
	
	# executables get an module registry file which contains the collected module loaders
	wiesel_create_module_registry(${target} ${MODULE_REGISTRY_FILE})

	# also create according test package
	# SORRY! no test support for executables yet :-/
	#wiesel_create_test_package_for(${target} "${test_dir}")
	
	# add the source dir as include directory to the target
	wiesel_target_add_includes(${target} ${source_dir})
endfunction(wiesel_create_executable)



function(wiesel_create_module target source_dir test_dir)
	# get all sources from this directory
	wiesel_module_get_files(MODULE_SRC_FILES ${source_dir})
	
	# add sources to the module's target
	add_library(${target} SHARED ${MODULE_SRC_FILES})
	
	# scan for all loadable modules in the library's source-tree
	wiesel_module_scan_for_loaders(MODULE_LOADERS ${source_dir})
	wiesel_module_export_loaders(${target} ${MODULE_LOADERS})
	
	# also create according test package
	wiesel_create_test_package_for(${target} "${test_dir}")
	
	# add the source dir as include directory to the target
	wiesel_target_add_includes(${target} ${source_dir})
endfunction(wiesel_create_module)



function(wiesel_target_add_includes target)
	set_property(TARGET ${target} APPEND PROPERTY INCLUDE_DIRECTORIES ${ARGN})
	
	# modify compiler flags
	foreach(incl ${ARGN})
		wiesel_target_add_compileflags(${target} -I${incl})
	endforeach()

	# also add the include to the according test target
	if (WIESEL_BUILD_TESTS)
		wiesel_target_has_valid_testpackage(HAS_TEST_PACKAGE ${target})
		wiesel_test_package_name(TEST_PACKAGE_NAME ${target})
		
		if (${HAS_TEST_PACKAGE})
			wiesel_target_add_includes(${TEST_PACKAGE_NAME} ${ARGN})
		endif()
	endif(WIESEL_BUILD_TESTS)
endfunction(wiesel_target_add_includes)



function(wiesel_target_add_libraries target)
	target_link_libraries(${target} ${ARGN})

	# also add the libraries to the according test target
	if (WIESEL_BUILD_TESTS)
		wiesel_target_has_valid_testpackage(HAS_TEST_PACKAGE ${target})
		wiesel_test_package_name(TEST_PACKAGE_NAME ${target})
		
		if (${HAS_TEST_PACKAGE})
			wiesel_target_add_libraries(${TEST_PACKAGE_NAME} ${ARGN})
		endif()
	endif(WIESEL_BUILD_TESTS)
endfunction(wiesel_target_add_libraries)



function(wiesel_target_add_compileflags target)
	# get the original compile flags
	get_property(TARGET_COMPILE_FLAGS TARGET ${target} PROPERTY COMPILE_FLAGS)
	
	# append each argument
	foreach(flag ${ARGN})
		set(TARGET_COMPILE_FLAGS "${TARGET_COMPILE_FLAGS} ${flag}")
	endforeach()
	
	# now write the flags property back
	set_property(TARGET ${target} PROPERTY COMPILE_FLAGS ${TARGET_COMPILE_FLAGS})

	# also add the compile flags to the according test target
	if (WIESEL_BUILD_TESTS)
		wiesel_target_has_valid_testpackage(HAS_TEST_PACKAGE ${target})
		wiesel_test_package_name(TEST_PACKAGE_NAME ${target})
		
		if (${HAS_TEST_PACKAGE})
			wiesel_target_add_compileflags(${TEST_PACKAGE_NAME} ${ARGN})
		endif()
	endif(WIESEL_BUILD_TESTS)
endfunction(wiesel_target_add_compileflags)
