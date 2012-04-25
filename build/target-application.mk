# target script for building an application


# macro for converting a source filename into a object filename
define get-object-file
$(BUILD_OBJ_DIR)/$(shell echo $(subst $(PROJECT_DIR),,$1) | sed -e 's/[\/\.]/_/g')$(OBJECTFILE_EXTENSION)
endef


# macro for defining the object file targets
define make-goal-cpp
$(strip $(shell echo $(shell $(COMPILE.C) $(BUILD_INCLUDE_PATHS) $(C_FLAGS) -MM -MQ"$2" $1) | sed -e 's/\s+/ /g'))
	@echo "Compiling $1 ...";
	@$(COMPILE.CPP) $(C_FLAGS) $(BUILD_INCLUDE_PATHS) -c $1 -o $2
endef


# collect all source folders
BUILD_SOURCE_FOLDERS		:= \
					$(WIESEL_BASE_SOURCE_FOLDERS)		\
					$(PROJECT_SOURCE_FOLDERS)		\


# get all directories within the source folders
BUILD_FOLDERS			:= $(foreach dir,$(BUILD_SOURCE_FOLDERS),$(shell /usr/bin/find $(dir) -type d -print))
BUILD_SOURCE_FILES		:= $(foreach dir,$(BUILD_FOLDERS),$(wildcard $(dir)/*.cpp))
BUILD_OBJECT_FILES		:= $(foreach file,$(BUILD_SOURCE_FILES),$(call get-object-file,$(file)))

# set the resulting filename
BUILD_TARGET			:= $(BUILD_BIN_DIR)/$(PROJECT_NAME)$(EXECUTABLE_EXTENSION)

# all source folders will be added to the include paths
BUILD_INCLUDE_PATHS		+= $(foreach dir,$(BUILD_SOURCE_FOLDERS),-I$(dir))



# define all object file targets
$(foreach file,$(BUILD_SOURCE_FILES),$(eval $(call make-goal-cpp,$(file),$(call get-object-file,$(file)))))


# create the output folders, if necessary
$(BUILD_OBJ_DIR):
	mkdir -p $(BUILD_OBJ_DIR)

$(BUILD_BIN_DIR):
	mkdir -p $(BUILD_BIN_DIR)




# build the dependency file
#$(BUILD_DEPENDENCY_FILE): $(BUILD_OBJ_DIR) $(BUILD_SOURCE_FILES) $(BUILD_SOURCE_H_FILES)
#	$(COMPILE.CPP) -MM $(BUILD_SOURCE_FILES) > $(BUILD_DEPENDENCY_FILE)
	

# get the dependencies
#include $(BUILD_DEPENDENCY_FILE)




# compiling the object files
#$(BUILD_OBJECT_FILES): $(BUILD_SOURCE_CPP_FILES)
#	@$(COMPILE.CPP) $(C_FLAGS) -c $+ -o $@

# compiling / linking the target
$(BUILD_TARGET): $(BUILD_BIN_DIR) $(BUILD_OBJ_DIR) $(BUILD_OBJECT_FILES)
	@$(LINK) $(LD_FLAGS) $(LD_LIBRARIES) $(BUILD_OBJECT_FILES) -o $(BUILD_TARGET)


build: $(BUILD_TARGET)


clean:
	rm -rf $(BUILD_BIN_DIR)
	rm -rf $(BUILD_OBJ_DIR)
	