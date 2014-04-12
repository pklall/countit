#include <string.h>
#include <jni.h>

#define cimg_display 0
#include "CImg.h"

jint
Java_com_example_countit_CounterJNIWrapper_processImage(
        JNIEnv* env,
        jobject thiz) {
    return 5;
}

