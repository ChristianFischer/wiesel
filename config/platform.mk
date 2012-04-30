# determine the current build-environment platform and architecture

ARCH			:= $(shell uname -m)
PLATFORM		:= $(shell uname)

# cygwin
ifeq ($(findstring CYGWIN,$(PLATFORM)),CYGWIN)
	PLATFORM	:= windows
endif

# mingw
ifeq ($(findstring MINGW32,$(PLATFORM)),MINGW32)
	PLATFORM	:= windows
endif

# linux
ifeq ($(findstring Linux,$(OS)),Linux)
	PLATFORM	:= linux
endif
