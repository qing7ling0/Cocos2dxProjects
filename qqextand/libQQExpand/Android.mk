LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos_qq_expand_static

LOCAL_MODULE_FILENAME := libqqexpand

LOCAL_SRC_FILES := 	\
WaterWaveAction.cpp \
GameLauncher.cpp \
Tools.cpp \
RecvThread.cpp \
SocketConn.cpp \
StreamIO.cpp \
Packet.cpp \
SocketThread.cpp \
SendThread.cpp \
Handler.cpp \
NetHandlers.cpp \
ODSocket.cpp \
Debug.cpp \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../editor-support

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../2d \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../.. \

LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,2d)
