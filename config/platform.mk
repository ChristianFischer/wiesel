# determine the current build-environment platform and architecture

ARCH			:= $(shell arch)
PLATFORM		:= $(shell uname)

# cygwin
ifeq ($(findstring CYGWIN,$(PLATFORM)),CYGWIN)
	PLATFORM	:= windows
endif

# linux
ifeq ($(findstring Linux,$(OS)),Linux)
	PLATFORM	:= linux
endif
