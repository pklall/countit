#include <string.h>
#include <jni.h>

#define cimg_display 0

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

            CImg<uint8_t> inputImg(imgWidth, imgHeight, 3);

            cimg_forXY(inputImg, x, y) {
                inputImg(x, y, 0) = rawInputBuf[(x + y * imgWidth) * 4 + 0];
                inputImg(x, y, 1) = rawInputBuf[(x + y * imgWidth) * 4 + 1];
                inputImg(x, y, 2) = rawInputBuf[(x + y * imgWidth) * 4 + 2];
            }

            CImg<uint8_t> outputImg(imgWidth, imgHeight, 3);

            outputImg = inputImg;

            for (int i = 0; i < numPoints; i++) {
                float x = ((float*) rawPointBuf)[i * 2 + 0];
                float y = ((float*) rawPointBuf)[i * 2 + 1];

                uint8_t color[3];
                color[0] = 255;
                color[1] = 0;
                color[2] = 0;

                outputImg.draw_circle(x, y, 5, color);
            }

            for (int y = 0; y < imgHeight; y++) {
                for (int x = 0; x < imgWidth; x++) {
                    // FIXME this only copies red channel
                    rawOverlayBuf[(x + y * imgWidth) * 4 + 0] = outputImg(x, y, 0);
                    rawOverlayBuf[(x + y * imgWidth) * 4 + 1] = outputImg(x, y, 1);
                    rawOverlayBuf[(x + y * imgWidth) * 4 + 2] = outputImg(x, y, 2);
                    rawOverlayBuf[(x + y * imgWidth) * 4 + 3] = 255;
                    // rawOverlayBuf[(x + y * imgWidth) * 4 + 1] = outputImg(x, y, 1);
                    // rawOverlayBuf[(x + y * imgWidth) * 4 + 2] = outputImg(x, y, 2);
                }
            }

            return 7;
        }

}
