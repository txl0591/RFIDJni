
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := zip
LOCAL_SRC_FILES := libzip.so
include $(PREBUILT_SHARED_LIBRARY)

