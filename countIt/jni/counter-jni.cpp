#include <string.h>
#include <jni.h>

#define cimg_display 0
#include "CImg.h"

#include "ibfs.h"

#include "GMM.h"

using namespace std;

using namespace cimg_library;

jint
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

    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < imgWidth; x++) {
            rawOverlayBuf[(x + y * imgWidth) * 4 + 0] = outputImg(x, y, 0);
            rawOverlayBuf[(x + y * imgWidth) * 4 + 1] = outputImg(x, y, 1);
            rawOverlayBuf[(x + y * imgWidth) * 4 + 2] = outputImg(x, y, 2);
        }
    }

    return 7;
}

