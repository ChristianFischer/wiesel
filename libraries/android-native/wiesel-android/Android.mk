# wiesel-android

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# define source folders of this library
MY_LOCAL_FOLDERS := \
				$(LOCAL_PATH)/src								\
				$(LOCAL_PATH)/../../../src/android				\

MY_SUBFOLDERS := \
				$(foreach dir,$(MY_LOCAL_FOLDERS),$(shell /usr/bin/find $(dir) -type d -print))

MY_FILES := \
				$(subst $(LOCAL_PATH), ,$(foreach dir,$(MY_SUBFOLDERS),$(wildcard $(dir)/*.cpp)))

# configure the module
LOCAL_MODULE                  := wiesel-android
LOCAL_MODULE_FILENAME         := libwieselandroid
LOCAL_CFLAGS                  := -Werror
LOCAL_LDLIBS                  := 
LOCAL_STATIC_LIBRARIES        := wiesel-base wiesel-core wiesel-opengl
LOCAL_WHOLE_STATIC_LIBRARIES  := android_native_app_glue
LOCAL_SRC_FILES               := $(MY_FILES)
LOCAL_C_INCLUDES              := $(MY_LOCAL_FOLDERS)
LOCAL_EXPORT_C_INCLUDES       := $(MY_LOCAL_FOLDERS)
LOCAL_EXPORT_LDLIBS           := -landroid -llog

# Build the static library
include $(BUILD_STATIC_LIBRARY)

# import all required modules
$(call import-module,wiesel-base)
$(call import-module,wiesel-core)
$(call import-module,wiesel-opengl)

# NativeActivity stuff
$(call import-module,android/native_app_glue)
