###                                               _    __ ____
 #   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 #  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 #  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 #  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 #
 ##

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := ParallelMERuntime
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -Ofast -Wall -Wextra -Werror
LOCAL_CPPFLAGS := -Ofast -Wall -Wextra -Werror -std=c++14 -fexceptions
LOCAL_CPP_FEATURES += exceptions
LOCAL_LDLIBS := -llog -ldl -ljnigraphics
LOCAL_SRC_FILES := src/parallelme/Buffer.cpp src/parallelme/Device.cpp \
	src/parallelme/Kernel.cpp src/parallelme/Program.cpp \
	src/parallelme/Runtime.cpp src/parallelme/Task.cpp \
	src/parallelme/SchedulerFCFS.cpp src/parallelme/SchedulerHEFT.cpp \
	src/parallelme/SchedulerPAMS.cpp src/parallelme/dynloader/dynLoader.c
include $(BUILD_SHARED_LIBRARY)
