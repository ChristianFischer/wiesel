
function(wiesel_module_scan_for_loaders var path)
	file(GLOB_RECURSE MY_FILES ${path}/*.loadable_module)
	set(${var} ${MY_FILES} PARENT_SCOPE)
endfunction(wiesel_module_scan_for_loaders)



function(wiesel_module_export_loaders module)
	set_property(TARGET ${module} APPEND PROPERTY EXPORT_MODULE_LOADERS ${ARGN})
endfunction(wiesel_module_export_loaders)



function(wiesel_create_module_registry target registry_file)
	# get the list of exported modules from the target
	get_target_property(MODULE_LOADERS ${target} EXPORT_MODULE_LOADERS)
	
	# create the file
	file(WRITE ${registry_file} "")
	
	# add each module to the registry
	if (NOT MODULE_LOADERS STREQUAL "MODULE_LOADERS-NOTFOUND")
		foreach(module ${MODULE_LOADERS})
			file(APPEND ${registry_file} "#include \"${module}\"")
			file(APPEND ${registry_file} "\n")
		endforeach()
	endif()

	# generated flag required, because otherwise cmake will report an error if the file is missing
	set_source_files_properties(${registry_file} PROPERTIES GENERATED TRUE)
endfunction(wiesel_create_module_registry)



function(wiesel_append_module_registry target registry_file)
	get_target_property(TARGET_TYPE ${target} TYPE)
	
	if (TARGET_TYPE STREQUAL "EXECUTABLE")
		# add each module to the registry
		foreach(module ${ARGN})
			file(APPEND ${registry_file} "#include \"${module}\"")
			file(APPEND ${registry_file} "\n")
		endforeach()
	endif()
endfunction(wiesel_append_module_registry)

