# linux 32bit configuration

# 32 bit architecture
C_FLAGS				+= -m32
LD_FLAGS			+= -m32

# c flags
C_FLAGS				+= 

# use SDL library
C_FLAGS				+= -DWIESEL_USE_LIBSDL
LD_LIBRARIES			+= SDL SDL_image GL
LD_LIBRARY_PATHS		+= 

OBJECTFILE_EXTENSION		:= .o
EXECUTABLE_EXTENSION		:= 
LIBRARY_EXTENSION		:= .a
SHARED_LIBRARY_EXTENSION	:= .so


