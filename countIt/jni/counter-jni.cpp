#include <string.h>
#include <jni.h>

#define cimg_display 0

#include <android/log.h>

#define APPNAME "CountIt"

#include "common.h"

extern "C" {
    JNIEXPORT
        jint
        JNICALL
        Java_com_example_countit_CounterJNIWrapper_jnitest(
                JNIEnv* env,
                jobject thiz) {
            return 3;
        }

    JNIEXPORT
        jint
        JNICALL
        Java_com_example_countit_CounterJNIWrapper_processImage(
                JNIEnv* env,
                jobject thiz,
                jint imgWidth,
                jint imgHeight,
                // A float buffer with interlaced (x, y) pairs indicating finger locations
                jint numPoints,
                jobject pointBuf,
                // ARGB8888 input image
                jobject imgInputBuf,
                // ARGB8888 output image
                jobject imgOverlayBuf) {

            uint8_t* rawPointBuf = (uint8_t*) env->GetDirectBufferAddress(pointBuf);
            uint8_t* rawInputBuf = (uint8_t*) env->GetDirectBufferAddress(imgInputBuf);
            uint8_t* rawOverlayBuf = (uint8_t*) env->GetDirectBufferAddress(imgOverlayBuf);

            CImg<uint8_t> inputImg(imgWidth, imgHeight, 1, 3);

            cimg_forXY(inputImg, x, y) {
                inputImg(x, y, 0, 0) = rawInputBuf[(x + y * imgWidth) * 4 + 0];
                inputImg(x, y, 0, 1) = rawInputBuf[(x + y * imgWidth) * 4 + 1];
                inputImg(x, y, 0, 2) = rawInputBuf[(x + y * imgWidth) * 4 + 2];
            }

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "initialized input image");

            CImg<bool> initialFg(inputImg.width(), inputImg.height());

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "allocated initialFG");

            initialFg = false;

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "initialized initialFG to false");

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "drawing circles to initFG...");

            for (int i = 0; i < numPoints; i++) {
                float x = ((float*) rawPointBuf)[i * 2 + 0];
                float y = ((float*) rawPointBuf)[i * 2 + 1];

                bool color = true;

                initialFg.draw_circle(x, y, 10, &color);
            }

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "done drawing circles to initFG");

            CImg<bool> outline(inputImg.width(), inputImg.height());

            int count;

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "computing...");

            nicename(inputImg, initialFg, outline, count);

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "done computing...");

            CImg<uint8_t> outputImg(imgWidth, imgHeight, 1, 3);

            outputImg = inputImg;

            for (int y = 0; y < imgHeight; y++) {
                for (int x = 0; x < imgWidth; x++) {
                    if (outline(x, y)) {
                        rawOverlayBuf[(x + y * imgWidth) * 4 + 0] = 255;
                        rawOverlayBuf[(x + y * imgWidth) * 4 + 1] = 255;
                        rawOverlayBuf[(x + y * imgWidth) * 4 + 2] = 255;
                    } else {
                        rawOverlayBuf[(x + y * imgWidth) * 4 + 0] = outputImg(x, y, 0, 0);
                        rawOverlayBuf[(x + y * imgWidth) * 4 + 1] = outputImg(x, y, 0, 1);
                        rawOverlayBuf[(x + y * imgWidth) * 4 + 2] = outputImg(x, y, 0, 2);
                    }

                    rawOverlayBuf[(x + y * imgWidth) * 4 + 3] = 255;
                }
            }

            return count;
        }

}
