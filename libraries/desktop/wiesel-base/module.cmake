# wiesel-base library


# create the library
wiesel_create_module(wiesel-base ${WIESEL_SRC_DIR}/base ${WIESEL_TESTS_DIR}/base)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-base ${WIESEL_SRC_DIR}/base)

# import thread API
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
include(FindThreads)

set(WIESEL_THREADAPI_PTHREAD		${CMAKE_USE_PTHREADS_INIT})
set(WIESEL_THREADAPI_WIN32			${CMAKE_USE_WIN32_THREADS_INIT})

target_link_libraries(wiesel-base ${CMAKE_THREAD_LIBS_INIT})

# check if at least one threadapi was found
if (
		NOT "${WIESEL_THREADAPI_PTHREAD}"
	AND	NOT "${WIESEL_THREADAPI_WIN32}"
)
	message(FATAL_ERROR "No supported threadapi found")
endif()


# finally, create the config file
configure_file(
		${WIESEL_SRC_DIR}/base/wiesel/wiesel-base-config.in
		${WIESEL_GENERATED_DIR}/wiesel/wiesel-base-config.h
)
