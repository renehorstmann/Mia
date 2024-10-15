LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main


#
# compile definitions
#
LOCAL_CFLAGS := -DMIA_PLATFORM_ANDROID -DMIA_OPTION_GLES -DMIA_OPTION_THREAD -DMIA_OPTION_FETCH
LOCAL_CFLAGS += -DMIA_TERMINALCOLOR_OFF -DMIA_LOG_COMPACT


LOCAL_CFLAGS += -DMIA_ANDROID_PACKAGE=@@@package_underscored@@@
LOCAL_CFLAGS += -DMIA_TITLE=@@@app_name@@@

# LOCAL_CFLAGS += -DMIA_OPTION_TTF
# LOCAL_CFLAGS += -DMIA_OPTION_SOCKET

# gamepad:
#LOCAL_CFLAGS += -DMIA_OPTION_GAMEPAD

# fullscreen
#@@@USE_FULLSCREEN@@@ LOCAL_CFLAGS += -DMIA_INIT_FULLSCREEN

# ads:
#@@@USE_ADMOB@@@ LOCAL_CFLAGS += -DMIA_OPTION_AD

# billing:
#@@@USE_BILLING@@@ LOCAL_CFLAGS += -DMIA_OPTION_BILLING


#
# includes
#

# ext includes
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

# mia includes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include


#
# source files
#

# Add your application source directories here...
MIA_SRC_FILES := $(wildcard $(LOCAL_PATH)/src/o/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/m/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/s/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/r/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/a/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/u/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/w/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/x/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/ex/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/mp/*.c)
MIA_SRC_FILES += $(wildcard $(LOCAL_PATH)/src/*.c)

LOCAL_SRC_FILES := $(MIA_SRC_FILES:$(LOCAL_PATH)/%=%)

#
# libraries used
#

# Also have a look at SDLActivity.java : getLibraries in which the modules are loaded (uncomment there, too)
LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_SHARED_LIBRARIES += SDL2_image
# LOCAL_SHARED_LIBRARIES += SDL2_mixer
# LOCAL_SHARED_LIBRARIES += SDL2_net
# LOCAL_SHARED_LIBRARIES += SDL2_ttf

# link libraries
#LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid
LOCAL_LDLIBS := -lEGL -lGLESv3 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
