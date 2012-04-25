# configure the default tool paths
# may be overridden in PROJECT_DIR/config/tools.mk

MAKE			:= $(shell which make)
COMPILE.C		:= $(shell which gcc)
COMPILE.CPP		:= $(shell which g++)
LINK			:= $(shell which g++)

# C compiler flags
C_FLAGS			:=

# C include directories
C_INCLUDE		:=

# C library search paths
C_LDLIBRARY_PATH	:=

# Libraries to be linked with the target
LD_LIBRARIES		:=

# linker flags
LD_FLAGS		:=