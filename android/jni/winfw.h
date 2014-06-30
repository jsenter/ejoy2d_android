#ifndef ejoy2d_windows_fw_h
#define ejoy2d_windows_fw_h

#include <jni.h>

#define WIDTH 1024
#define HEIGHT 768

#define TOUCH_BEGIN 0
#define TOUCH_END 1
#define TOUCH_MOVE 2

void ejoy2d_win_init(JNIEnv * env, jobject asset, int w, int h);
void ejoy2d_win_frame();
void ejoy2d_win_update();
void ejoy2d_win_touch(float x, float y,int touch);

#endif