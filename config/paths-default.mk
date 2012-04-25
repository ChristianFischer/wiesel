# default paths - may be overridden in PROJECT_DIR/config/paths.mk

# project paths
PROJECT_SOURCE_FOLDERS	:= $(PROJECT_DIR)/src

# build paths
BUILD_SUBDIR		:= $(PLATFORM)-$(ARCH)
BUILD_OBJ_DIR		:= $(PROJECT_DIR)/build/$(BUILD_SUBDIR)/obj
BUILD_BIN_DIR		:= $(PROJECT_DIR)/build/$(BUILD_SUBDIR)/bin

