# libpng static library

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# define source folders of this library
LIBXML_VERSION             := 2.8.0
LIBXML_RELATIVE_FOLDER     := ../../../../src/third-party/libxml2
LIBXML_SRC_FOLDER          := $(LOCAL_PATH)/$(LIBXML_RELATIVE_FOLDER)
LIBXML_INCLUDE_FOLDERS     := $(LOCAL_PATH)/src/include $(LIBXML_SRC_FOLDER)/include
#LIBXML_SRC_FILES           := $(wildcard $(LIBXML_SRC_FOLDER)/*.c)
LIBXML_SRC_FILES           := \
								$(LIBXML_RELATIVE_FOLDER)/c14n.c				\
								$(LIBXML_RELATIVE_FOLDER)/catalog.c				\
								$(LIBXML_RELATIVE_FOLDER)/chvalid.c				\
								$(LIBXML_RELATIVE_FOLDER)/debugXML.c			\
								$(LIBXML_RELATIVE_FOLDER)/dict.c				\
								$(LIBXML_RELATIVE_FOLDER)/DOCBparser.c			\
								$(LIBXML_RELATIVE_FOLDER)/encoding.c			\
								$(LIBXML_RELATIVE_FOLDER)/entities.c			\
								$(LIBXML_RELATIVE_FOLDER)/error.c				\
								$(LIBXML_RELATIVE_FOLDER)/globals.c				\
								$(LIBXML_RELATIVE_FOLDER)/hash.c				\
								$(LIBXML_RELATIVE_FOLDER)/HTMLparser.c			\
								$(LIBXML_RELATIVE_FOLDER)/HTMLtree.c			\
								$(LIBXML_RELATIVE_FOLDER)/legacy.c				\
								$(LIBXML_RELATIVE_FOLDER)/list.c				\
								$(LIBXML_RELATIVE_FOLDER)/nanoftp.c				\
								$(LIBXML_RELATIVE_FOLDER)/nanohttp.c			\
								$(LIBXML_RELATIVE_FOLDER)/parser.c				\
								$(LIBXML_RELATIVE_FOLDER)/parserInternals.c		\
								$(LIBXML_RELATIVE_FOLDER)/pattern.c				\
								$(LIBXML_RELATIVE_FOLDER)/relaxng.c				\
								$(LIBXML_RELATIVE_FOLDER)/SAX.c					\
								$(LIBXML_RELATIVE_FOLDER)/SAX2.c				\
								$(LIBXML_RELATIVE_FOLDER)/schematron.c			\
								$(LIBXML_RELATIVE_FOLDER)/threads.c				\
								$(LIBXML_RELATIVE_FOLDER)/tree.c				\
								$(LIBXML_RELATIVE_FOLDER)/uri.c					\
								$(LIBXML_RELATIVE_FOLDER)/valid.c				\
								$(LIBXML_RELATIVE_FOLDER)/xinclude.c			\
								$(LIBXML_RELATIVE_FOLDER)/xlink.c				\
								$(LIBXML_RELATIVE_FOLDER)/xmlcatalog.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlIO.c				\
								$(LIBXML_RELATIVE_FOLDER)/xmllint.c				\
								$(LIBXML_RELATIVE_FOLDER)/xmlmemory.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlmodule.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlreader.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlregexp.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlsave.c				\
								$(LIBXML_RELATIVE_FOLDER)/xmlschemas.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlschemastypes.c		\
								$(LIBXML_RELATIVE_FOLDER)/xmlstring.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlunicode.c			\
								$(LIBXML_RELATIVE_FOLDER)/xmlwriter.c			\
								$(LIBXML_RELATIVE_FOLDER)/xpath.c				\
								$(LIBXML_RELATIVE_FOLDER)/xpointer.c			\
								$(LIBXML_RELATIVE_FOLDER)/xzlib.c				\
								
# excluded:
#								$(LIBXML_RELATIVE_FOLDER)/rngparser.c
#								$(LIBXML_RELATIVE_FOLDER)/trio.c
#								$(LIBXML_RELATIVE_FOLDER)/trionan.c
#								$(LIBXML_RELATIVE_FOLDER)/triostr.c

# configure the module
LOCAL_MODULE               := libxml2-static
LOCAL_MODULE_FILENAME      := libxml2-$(LIBXML_VERSION)-static
LOCAL_CFLAGS               := -Werror
LOCAL_LDLIBS               := 
LOCAL_STATIC_LIBRARIES     := 
LOCAL_SRC_FILES            := $(LIBXML_SRC_FILES)
LOCAL_C_INCLUDES           := $(LIBXML_INCLUDE_FOLDERS) $(LOCAL_PATH)/src/config
LOCAL_EXPORT_C_INCLUDES    := $(LIBXML_INCLUDE_FOLDERS)

# Build the static library
include $(BUILD_STATIC_LIBRARY)
