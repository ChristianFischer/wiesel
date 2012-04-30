# target script for building an application


# macro for converting a source filename into a object filename
define get-object-file
$(BUILD_OBJ_DIR)/$(subst .,_,$(subst /,_,$(subst $(PROJECT_DIR),,$1)))$(OBJECTFILE_EXTENSION)
endef


# macro for defining the object file targets
define make-goal-c
$(strip $(shell echo $(shell $(COMPILE.CPP) $(C_FLAGS) $(COMPILER_ARGUMENTS) -MM -MQ"$2" $1) | sed -e 's/\s+/ /g'))
	@echo "Compiling $1 ...";
	@$(COMPILE.CPP) $(C_FLAGS) $(COMPILER_ARGUMENTS) -c $1 -o $2
endef

# same for cpp targets
define make-goal-cpp
$(strip $(shell echo $(shell $(COMPILE.C) $(C_FLAGS) $(COMPILER_ARGUMENTS) -MM -MQ"$2" $1) | sed -e 's/\s+/ /g'))
	@echo "Compiling $1 ...";
	@$(COMPILE.C) $(C_FLAGS) $(COMPILER_ARGUMENTS) -c $1 -o $2
endef


# get all directories within the source folders
BUILD_FOLDERS			:= $(foreach dir,$(PROJECT_SOURCE_FOLDERS),$(shell /usr/bin/find $(dir) -type d -print))
BUILD_SOURCE_H_FILES		:= $(foreach dir,$(BUILD_FOLDERS),$(wildcard $(dir)/*.h))
BUILD_SOURCE_C_FILES		:= $(foreach dir,$(BUILD_FOLDERS),$(wildcard $(dir)/*.c))
BUILD_SOURCE_CPP_FILES		:= $(foreach dir,$(BUILD_FOLDERS),$(wildcard $(dir)/*.cpp))
BUILD_SOURCE_FILES		:= $(BUILD_SOURCE_C_FILES) $(BUILD_SOURCE_CPP_FILES)
BUILD_OBJECT_FILES		:= $(foreach file,$(BUILD_SOURCE_FILES),$(call get-object-file,$(file)))

# set the name of the dependency file
BUILD_DEPENDENCY_FILE		:= $(BUILD_OBJ_DIR)/.dep.inc.mk

# set the resulting filename
BUILD_TARGET			:= $(BUILD_BIN_DIR)/$(PROJECT_NAME)$(EXECUTABLE_EXTENSION)

# all source folders will be added to the include paths
C_INCLUDE_PATHS			+= $(PROJECT_SOURCE_FOLDERS)



# create commandline arguments

# compiler arguments
COMPILER_ARGUMENTS		+= $(foreach dir,$(C_INCLUDE_PATHS),-I$(dir))

# linker library paths
LINKER_ARGUMENTS		+= $(foreach dir,$(LD_LIBRARY_PATHS),-L$(dir))

# extra libraries
LINKER_ARGUMENTS		+= $(foreach dir,$(LD_LIBRARIES),-l$(dir))



# define all object file targets
$(foreach file,$(BUILD_SOURCE_C_FILES),$(eval $(call make-goal-c,$(file),$(call get-object-file,$(file)))))
$(foreach file,$(BUILD_SOURCE_CPP_FILES),$(eval $(call make-goal-cpp,$(file),$(call get-object-file,$(file)))))


# create the output folders, if necessary
$(BUILD_OBJ_DIR):
	@mkdir -p $(BUILD_OBJ_DIR)

$(BUILD_BIN_DIR):
	@mkdir -p $(BUILD_BIN_DIR)




# build the dependency file
$(BUILD_DEPENDENCY_FILE): $(BUILD_OBJ_DIR) $(BUILD_SOURCE_FILES) $(BUILD_SOURCE_H_FILES)
	@echo "computing dependencies...";
	@echo "# build dependency file" > $(BUILD_DEPENDENCY_FILE);
	@for file in $(BUILD_SOURCE_FILES); do \
		echo "# $$file" >> $(BUILD_DEPENDENCY_FILE); \
		$(COMPILE.CPP) $(C_FLAGS) $(COMPILER_ARGUMENTS) \
		-MM -MQ"$$file$(OBJECTFILE_EXTENSION)" $$file >> $(BUILD_DEPENDENCY_FILE); \
		echo "" >> $(BUILD_DEPENDENCY_FILE); \
	done;

# get the dependencies
#include $(BUILD_DEPENDENCY_FILE)




# compiling the object files
#$(BUILD_OBJECT_FILES): $(BUILD_SOURCE_CPP_FILES)
#	@$(COMPILE.CPP) $(C_FLAGS) -c $+ -o $@

# compiling / linking the target
$(BUILD_TARGET): $(BUILD_BIN_DIR) $(BUILD_OBJ_DIR) $(BUILD_OBJECT_FILES)
	@echo "Linking...";
	@$(LINK) $(BUILD_OBJECT_FILES) -o $(BUILD_TARGET) $(LD_FLAGS) $(LINKER_ARGUMENTS)


# list the current configuration
show-config:
	@echo "Make:             $(MAKE) - $(shell $(MAKE) --version)";
	@echo "C Compiler:       $(COMPILE.C) - $(shell $(COMPILE.C) --version)";
	@echo "C++ Compiler:     $(COMPILE.CPP) - $(shell $(COMPILE.CPP) --version)";
	@echo "Linker:           $(LINK) - $(shell $(LINK) --version)";
	@echo "";
	
	@echo "Compiler Flags:   $(C_FLAGS)";
	@echo "Linker Flags:     $(LD_FLAGS)";
	@echo "Extra Libraries:  $(LD_LIBRARIES)";
	@echo "";
	
	@echo "Source Folders:";
	@for dir in $(PROJECT_SOURCE_FOLDERS); do \
		echo "    - $$dir"; \
		if [ ! -d $$dir ]; then \
			echo "      !! directory not reachable!"; \
		fi; \
	done;
	@echo "";
	
	@echo "Include directories:";
	@for dir in $(C_INCLUDE_PATHS); do \
		echo "    - $$dir"; \
		if [ ! -d $$dir ]; then \
			echo "      !! directory not reachable!"; \
		fi; \
	done;
	@echo "";
	
	@echo "Library search paths:";
	@for dir in $(LD_LIBRARY_PATHS); do \
		echo "    - $$dir"; \
		if [ ! -d $$dir ]; then \
			echo "      !! directory not reachable!"; \
		fi; \
	done;
	@echo "";


# create the dependency file
dep: $(BUILD_DEPENDENCY_FILE)


# build the application
build: $(BUILD_TARGET)


# clear all build files
clean:
	@rm -rf $(BUILD_BIN_DIR)
	@rm -rf $(BUILD_OBJ_DIR)

