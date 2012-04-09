# libpng static library

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# define source folders of this library
LIBPNG_VERSION             := 1.5.10
LIBPNG_RELATIVE_FOLDER     := ../../../../src/third-party/libpng-$(LIBPNG_VERSION)
LIBPNG_SRC_FOLDER          := $(LOCAL_PATH)/$(LIBPNG_RELATIVE_FOLDER)
LIBPNG_SRC_FILES           :=											\
							$(LIBPNG_RELATIVE_FOLDER)/png.c				\
							$(LIBPNG_RELATIVE_FOLDER)/pngerror.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngget.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngmem.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngpread.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngread.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngrio.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngrtran.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngrutil.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngset.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngtest.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngtrans.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngwio.c			\
							$(LIBPNG_RELATIVE_FOLDER)/pngwrite.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngwtran.c		\
							$(LIBPNG_RELATIVE_FOLDER)/pngwutil.c		\

# configure the module
LOCAL_MODULE               := libpng-static
LOCAL_MODULE_FILENAME      := libpng-$(LIBPNG_VERSION)-static
LOCAL_CFLAGS               := -Werror
LOCAL_LDLIBS               := 
LOCAL_STATIC_LIBRARIES     := 
LOCAL_SRC_FILES            := $(LIBPNG_SRC_FILES)
LOCAL_C_INCLUDES           := $(LIBPNG_SRC_FOLDER)
LOCAL_EXPORT_C_INCLUDES    := $(LIBPNG_SRC_FOLDER)
LOCAL_EXPORT_LDLIBS        := -lz

# Build the static library
include $(BUILD_STATIC_LIBRARY)
