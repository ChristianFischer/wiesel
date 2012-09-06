# makefile for the current project

# ensure a correct version of cmake is installed
cmake_minimum_required(VERSION 2.8)


# in this case, the engine folder is in the same repository,
# so we always know the correct location.
# when the engine location is different on various build environments,
# you may put the definition of WIESEL_DIR into a separate file under 
# config/local, where it is not committed into version control.
set(WIESEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# include the main build file of the engine
include(${WIESEL_DIR}/build/build.cmake)



# set the project name
project(HelloWiesel)



wiesel_create_executable(HelloWiesel src *.cpp)


# add required libraries
wiesel_module_add_dependency(HelloWiesel wiesel-base)
wiesel_module_add_dependency(HelloWiesel wiesel-common)

wiesel_target_add_compileflags(HelloWiesel "-DWIESEL_USE_LIBSDL")

