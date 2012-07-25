# HelloWiesel

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# add the path to the libraries into search path
WIESEL_PATH := $(LOCAL_PATH)/../$(WIESEL_DIR)/libraries/android-native
$(call import-add-path,$(WIESEL_PATH))

# define the source folders of this project
MY_LOCAL_FOLDERS := \
				$(LOCAL_PATH)									\
				$(LOCAL_PATH)/../../src							\
				

MY_SUBFOLDERS := \
				$(foreach dir,$(MY_LOCAL_FOLDERS),$(shell /usr/bin/find $(dir) -type d -print))

MY_FILES := \
				$(subst $(LOCAL_PATH), ,$(foreach dir,$(MY_SUBFOLDERS),$(wildcard $(dir)/*.cpp)))

# configure the application's build
LOCAL_MODULE               := main
LOCAL_SRC_FILES            := $(MY_FILES)
LOCAL_LDLIBS               :=
LOCAL_STATIC_LIBRARIES     := wiesel-common

# build the shared library
include $(BUILD_SHARED_LIBRARY)

# import all required modules
$(call import-module,wiesel-base)
$(call import-module,wiesel-common)
