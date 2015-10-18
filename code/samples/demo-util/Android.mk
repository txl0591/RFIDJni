LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
  
LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(LOCAL_PATH)/../../include	\
	$(LOCAL_PATH)/../../ThirdParty/libzip	\

#include $(LOCAL_PATH)/../../../jni/Config.mk
	
LOCAL_MODULE := demo-util 

LOCAL_SRC_FILES += \
    demo-util.c \

LOCAL_SHARED_LIBRARIES := libRFIDJni	\
			libzip	\
			

LOCAL_LDLIBS := -llog
include $(BUILD_EXECUTABLE)
 
