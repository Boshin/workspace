#### START ####
include usr/config/config.mk
LOCAL_PROJECT_PATH := $(ROOT_SRC)/mysqlite
### END ###

usr/config/config.mk
### START ###
ROOT_PATH := $(PWD)
ROOT_SRC := $(ROOT_PATH)/sources
#此变量根据不同环境配置
HOME_PATH := 

ANDROID_SRC := $(HOME_PATH)/android
ANDROID_FRAMEWORKS_BASE_INCLUDE := $(ANDROID_SRC)/frameworks/base/include
ANDROID_SYSTEM_CORE_INCLUDE := $(ANDROID_SRC)/system/core/include
ANDROID_DALVIK_INCLUDE := $(ANDROID_SRC)/dalvik/libnativehelper/include
ANDROID_EXTERNAL := $(ANDROID_SRC)/external
ANDROID_EXTERNAL_SQLITE_INCLUDE := $(ANDROID_EXTERNAL)/sqlite/dist

ANDROID_LIB := $(ANDROID_SRC)/out/target/product/generic/system/lib

### END ###