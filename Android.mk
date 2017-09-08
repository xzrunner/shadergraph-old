INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := shaderlab

LOCAL_C_INCLUDES := \
	${LUA_SRC_PATH} \
	${SM_SRC_PATH} \
	${EJOY2D_SRC_PATH} \
	${DS_SRC_PATH} \
	${CU_SRC_PATH} \
	${SHADERLAB_SRC_PATH}/include \
	${SHADERLAB_SRC_PATH}/include/shaderlab \
	${UNIRENDER_SRC_PATH}/include \
	${COOKING_SRC_PATH}/include \
	${MULTITASK_SRC_PATH}/include \

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/source/ -name "*.cpp" -print)) \

include $(BUILD_STATIC_LIBRARY)	

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)