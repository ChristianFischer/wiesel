# Main build file for compiling projects based on this engine

# phony targets
.PHONY: clean dep

# set some basic paths
WIESEL_CONFIG_DIR		:= $(WIESEL_DIR)/config
WIESEL_BUILDSCRIPTS_DIR		:= $(WIESEL_DIR)/build
PROJECT_CONFIG_DIR		:= $(PROJECT_DIR)/config
PROJECT_LOCAL_CONFIG_DIR	:= $(PROJECT_DIR)/config/local

# when config is not set, set the default config to 'debug'
ifeq ($(CONFIGURATION),)
	CONFIGURATION		:= debug
endif

# determine architecture and platform
include $(WIESEL_CONFIG_DIR)/platform.mk

# path settings of the project
 include $(WIESEL_CONFIG_DIR)/paths-default.mk
-include $(PROJECT_CONFIG_DIR)/paths.mk
-include $(PROJECT_LOCAL_CONFIG_DIR)/paths.mk

# get the tools configuration
 include $(WIESEL_CONFIG_DIR)/tools-default.mk
-include $(PROJECT_CONFIG_DIR)/tools.mk
-include $(PROJECT_LOCAL_CONFIG_DIR)/tools.mk

# include the platform configuration
 include $(WIESEL_CONFIG_DIR)/config-$(PLATFORM)-$(ARCH).mk
-include $(PROJECT_CONFIG_DIR)/config-$(PLATFORM)-$(ARCH).mk
-include $(PROJECT_LOCAL_CONFIG_DIR)/config-$(PLATFORM)-$(ARCH).mk

 include $(WIESEL_CONFIG_DIR)/config-$(CONFIGURATION).mk
-include $(PROJECT_CONFIG_DIR)/config-$(CONFIGURATION).mk
-include $(PROJECT_LOCAL_CONFIG_DIR)/config-$(CONFIGURATION).mk

# get the makefile-includes of each library
include $(WIESEL_DIR)/libraries/desktop/*/Makefile.mk

# finally, include our target scripts
include $(WIESEL_BUILDSCRIPTS_DIR)/target-application.mk

