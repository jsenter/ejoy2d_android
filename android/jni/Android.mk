# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libgl2jni
LOCAL_CFLAGS    := -Werror -Dlinux -D"getlocaledecpoint()='.'" -D__ANDROID__
#VisualGDBAndroid: AutoUpdateSourcesInNextLine
LOCAL_SRC_FILES := 	gl_code.c \
					android_font.c \
					winfw.c \
					../../lib/shader.c \
					../../lib/lshader.c \
					../../lib/ejoy2dgame.c \
					../../lib/fault.c \
					../../lib/screen.c \
					../../lib/texture.c \
					../../lib/ppm.c \
					../../lib/spritepack.c \
					../../lib/sprite.c \
					../../lib/lsprite.c \
					../../lib/matrix.c \
					../../lib/lmatrix.c \
					../../lib/dfont.c \
					../../lib/label.c \
					../../lib/particle.c \
					../../lib/lparticle.c \
					../../lib/scissor.c \
					../../lua/lapi.c \
					../../lua/lauxlib.c \
					../../lua/lbaselib.c \
					../../lua/lbitlib.c \
					../../lua/lcode.c \
					../../lua/lcorolib.c \
					../../lua/lctype.c \
					../../lua/ldblib.c \
					../../lua/ldebug.c \
					../../lua/ldo.c \
					../../lua/ldump.c \
					../../lua/lfunc.c \
					../../lua/lgc.c \
					../../lua/linit.c \
					../../lua/liolib.c \
					../../lua/llex.c \
					../../lua/lmathlib.c \
					../../lua/lmem.c \
					../../lua/loadlib.c \
					../../lua/lobject.c \
					../../lua/lopcodes.c \
					../../lua/loslib.c \
					../../lua/lparser.c \
					../../lua/lstate.c \
					../../lua/lstring.c \
					../../lua/lstrlib.c \
					../../lua/ltable.c \
					../../lua/ltablib.c \
					../../lua/ltm.c \
					../../lua/lundump.c \
					../../lua/lvm.c \
					../../lua/lzio.c
					
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../lib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lua
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../freetype2/include/android
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../freetype2/include/android/freetype2

LOCAL_LDLIBS := -llog -lGLESv2 -lz -lm -landroid

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_freetype2_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_png_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module, freetype2/prebuilt/android)
$(call import-module, png/prebuilt/android)
