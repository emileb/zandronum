
LOCAL_PATH := $(call my-dir)/../lzma

include $(CLEAR_VARS)

LOCAL_MODULE    := lzma_zan31

LOCAL_CFLAGS = -Wall -fomit-frame-pointer -D_7ZIP_ST

LOCAL_C_INCLUDES :=

LOCAL_SRC_FILES =  \
    C/7zBuf.c \
	C/7zCrc.c \
	C/7zCrcOpt.c \
	C/7zDec.c \
	C/7zIn.c \
	C/7zStream.c \
	C/Bcj2.c \
	C/Bra.c \
	C/Bra86.c \
	C/CpuArch.c \
	C/LzFind.c \
	C/Lzma2Dec.c \
	C/LzmaDec.c \
	C/LzmaEnc.c \

LOCAL_CFLAGS += -fvisibility=hidden -fdata-sections -ffunction-sections

include $(BUILD_STATIC_LIBRARY)








