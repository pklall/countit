#include <string.h>
#include <jni.h>

#define cimg_display 0
#include "CImg.h"

#include "ibfs.h"

#include "GMM.h"

jint
Java_com_example_countit_CounterJNIWrapper_processImage(
        JNIEnv* env,
        jobject thiz) {
    return 5;
}

