
option(WIESEL_BUILD_TESTS "Adds testing support to the current build." ON)

if(WIESEL_BUILD_TESTS)
	# enable testing for the whole project
	enable_testing()

	# we require the ExternalProject module
	include(ExternalProject)

	# set CppUnit source properties
	set(CPPUNIT_DOWNLOAD_URL				"http://sourceforge.net/projects/cppunit/files/cppunit/1.12.1/cppunit-1.12.1.tar.gz/download")
	set(CPPUNIT_DOWNLOAD_MD5				"bd30e9cf5523cdfc019b94f5e1d7fd19")
	set(CPPUNIT_SVN_CHECKOUT_URL			"https://cppunit.svn.sourceforge.net/svnroot/cppunit")
	set(CPPUNIT_SVN_CHECKOUT_REVISION		"HEAD")

	# set Googletest source properties
	set(GOOGLETEST_DOWNLOAD_URL				"http://googletest.googlecode.com/files/gtest-1.6.0.zip")
	set(GOOGLETEST_DOWNLOAD_MD5				"4577b49f2973c90bf9ba69aa8166b786")
	set(GOOGLETEST_SVN_CHECKOUT_URL			"http://googletest.googlecode.com/svn/trunk/")
	set(GOOGLETEST_SVN_CHECKOUT_REVISION	"HEAD")

	set(GOOGLETEST_CMAKE_ARGS
	)
	
	# for MinGW we need to disable threading, because it's missing some POSIX functions
	if (MINGW OR MSYS)
		set(GOOGLETEST_ADD_PTHREADS_SUPPORT	FALSE)
	else()
		set(GOOGLETEST_ADD_PTHREADS_SUPPORT	TRUE)
	endif(MINGW OR MSYS)

	# disable pthread support on googletest
	if (NOT GOOGLETEST_ADD_PTHREADS_SUPPORT)
		set(
				GOOGLETEST_CMAKE_ARGS
				${GOOGLETEST_CMAKE_ARGS}
				-Dgtest_disable_pthreads=ON
		)
	endif()


	# get external project
	# the source of GoogleTest should be downloaded from the original SVN repository
	# if SVN is not available, try the download URL
	if (Subversion_SVN_EXECUTABLE)
		ExternalProject_Add(
				GoogleTest
				PREFIX				GoogleTest
				SVN_REPOSITORY		${GOOGLETEST_SVN_CHECKOUT_URL}
				SVN_REVISION		${GOOGLETEST_SVN_CHECKOUT_REVISION}

				# cmake config
				CMAKE_GENERATOR		"${CMAKE_GENERATOR}"
				CMAKE_ARGS			${GOOGLETEST_CMAKE_ARGS}

				# skip installation
				INSTALL_COMMAND 	""
		)
	else()
		ExternalProject_Add(
				GoogleTest
				PREFIX				GoogleTest
				URL					${GOOGLETEST_DOWNLOAD_URL}
				URL_MD5				${GOOGLETEST_DOWNLOAD_MD5}

				# cmake config
				CMAKE_GENERATOR		"${CMAKE_GENERATOR}"
				CMAKE_ARGS			"${GOOGLETEST_CMAKE_ARGS}"

				# skip installation
				INSTALL_COMMAND 	""
		)
	endif()


	# get GoogleTest directories
	ExternalProject_Get_Property(GoogleTest SOURCE_DIR)
	ExternalProject_Get_Property(GoogleTest BINARY_DIR)
	
	# set GoogleTest include & libraries
	set(GOOGLETEST_INCLUDE_DIR       ${SOURCE_DIR}/include)
	set(GOOGLETEST_LINK_LIBRARIES    ${BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
	
endif(WIESEL_BUILD_TESTS)




function(wiesel_create_test_package_for target test_dir)
	if (WIESEL_BUILD_TESTS AND (NOT "${test_dir}" STREQUAL ""))
		# get test name and all sources from this directory
		wiesel_test_package_name(TEST_NAME ${target})
		wiesel_module_get_files(TEST_SRC_FILES ${test_dir})

		# create test runner
		add_executable(${TEST_NAME} ${WIESEL_TESTS_DIR}/testrunner.cpp ${TEST_SRC_FILES})

		# add include directories
		wiesel_target_add_includes(${TEST_NAME} ${GOOGLETEST_INCLUDE_DIR})

		# link this library to the GoogleTest libs
		add_dependencies(${TEST_NAME} GoogleTest)
		target_link_libraries(${TEST_NAME} ${GOOGLETEST_LINK_LIBRARIES})
		wiesel_target_add_includes(${TEST_NAME} ${GOOGLETEST_INCLUDE_DIR})
		
		# add dependency to it's module target
		add_dependencies(${TEST_NAME} ${target})
		target_link_libraries(${TEST_NAME} ${target})

		# link pthread library if required
		if (GOOGLETEST_ADD_PTHREADS_SUPPORT)
			target_link_libraries(${TEST_NAME} pthread)
		endif(GOOGLETEST_ADD_PTHREADS_SUPPORT)

		# finally create the test
		add_test(
				NAME				${target}
				WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}
				COMMAND				${CMAKE_BINARY_DIR}/${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX}
		)
	endif()
endfunction(wiesel_create_test_package_for)



function(wiesel_test_package_name var target)
	set(${var} "runtests-${target}" PARENT_SCOPE)
endfunction(wiesel_test_package_name)



function(wiesel_is_test_package var target)
	string(FIND "${target}" "runtests-" tmp)
	if (tmp EQUAL -1)
		set(${var} TRUE PARENT_SCOPE)
	else()
		set(${var} FALSE PARENT_SCOPE)
	endif()
endfunction(wiesel_is_test_package)



function(wiesel_target_has_valid_testpackage var target)
	wiesel_is_test_package(IS_TEST_PACKAGE ${target})

	if((IS_TEST_PACKAGE) AND (NOT ${target} STREQUAL "TestRunner") AND (TARGET "runtests-${target}"))
		set(${var} TRUE PARENT_SCOPE)
	else()
		set(${var} FALSE PARENT_SCOPE)
	endif()
endfunction(wiesel_target_has_valid_testpackage)

