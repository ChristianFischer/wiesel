# windows 32bit configuration

# 32 bit architecture
C_FLAGS				+= -m32 -mwindows
LD_FLAGS			+= -m32 -mwindows

# windows
C_FLAGS				+= -D__WIN32__ -D_WIN32 -DWIN32

# use SDL library
C_FLAGS				+= -DWIESEL_USE_LIBSDL
LD_LIBRARIES			+= SDL opengl32
LD_LIBRARY_PATHS		+= 

OBJECTFILE_EXTENSION		:= .obj
EXECUTABLE_EXTENSION		:= .exe
LIBRARY_EXTENSION		:= .lib
SHARED_LIBRARY_EXTENSION	:= .dll


