#### START ####
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
#include $(LOCAL_PATH)/../config.mk

LOCAL_SRC_FILES:= test.cpp


LOCAL_LDLIBS := -lcutils -licuuc -licui18n -lutils -licudata -lsqlite 


#LOCAL_PRELINK_MODULE := false

LOCAL_MODULE :=  sqlite3

include $(BUILD_EXECUTABLE)

### END ###