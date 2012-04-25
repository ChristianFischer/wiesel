# makefile for the current project

# set the project name
PROJECT_NAME := HelloWiesel

# get the current project's directory
PROJECT_DIR := $(shell pwd)

# in this case, the engine folder is in the same repository,
# so we always know the correct location.
# when the engine location is different on various build environments,
# you may put the definition of WIESEL_DIR into a separate file under 
# config/local, where it is not committed into version control.
WIESEL_DIR := $(PROJECT_DIR)/../..

# include the main build file of the engine
include $(WIESEL_DIR)/Makefile
