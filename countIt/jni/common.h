#include "Eigen/Dense"

#include <string.h>
#include <stdio.h>

#include "CImg.h"

#include "ibfs.h"

#include "GMM.h"

#include "scribble.h"

#include <limits>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>

using namespace std;

using namespace cimg_library;

void refineGMM(
        const CImg<float>& labImg,
        const CImg<float>& lfg,
        const CImg<float>& lbg,
        const CImg<bool>& fgMask,
        GMM& fgGMM,
        float inlierProportion) {
    vector<tuple<float, int, int>> samples;

    cimg_forXY(labImg, x, y) {
        if (fgMask(x, y)) {
            float diff = lfg(x, y) - lbg(x, y);

            if (diff > 0) {
                samples.push_back(make_tuple(diff, x, y));
            }
        }
    }

    std::sort(samples.begin(), samples.end());

    for (int i = 0; i < samples.size() * inlierProportion; i++) {
        const int& x = get<1>(samples[i]);
        const int& y = get<2>(samples[i]);

        float color[3];
        color[0] = labImg(x, y, 0, 0);
        color[1] = labImg(x, y, 0, 1);
        color[2] = labImg(x, y, 0, 2);

        fgGMM.insertData(color, 1);
    }
}

void eraseDisconnectedBg(
        CImg<bool>& fgMask) {
    CImg<int> labels = fgMask.get_label();

    int bgLabel = labels(0, 0);

    cimg_forXY(fgMask, x, y) {
        if (!fgMask(x, y) && labels(x, y) != bgLabel) {
            fgMask(x, y) = true;
        }
    }
}

int countConnComponents(
        CImg<bool>& fgMask,
        int minPixelCount) {
    CImg<int> labels = fgMask.get_label();

    map<int, int> pixelCounts;

    cimg_forXY(fgMask, x, y) {
        if (fgMask(x, y)) {
            int label = labels(x, y);

            if (pixelCounts.count(label) == 0) {
                pixelCounts[label] = 0;
            }

            pixelCounts[label]++;
        }
    }

    cimg_forXY(fgMask, x, y) {
        if (fgMask(x, y)) {
            int label = labels(x, y);
            if (pixelCounts[label] < minPixelCount) {
                fgMask(x, y) = false;
            }
        }
    }

    int count = 0;

    for (const auto& pair : pixelCounts) {
        if (pair.second > minPixelCount) {
            count++;
        }
    }

    return count;
}

void createOutline(
        const CImg<bool>& fgMask,
        CImg<bool>& outline) {
    outline = fgMask.get_dilate(3) - fgMask.get_erode(1);
}

void nicename(
        const CImg<uint8_t>& img,
        const CImg<bool>& initialFg,
        CImg<bool>& outline,
        int& count) {
    const int numGaussians = 5;

    GMM fgGMM(3, numGaussians);
    GMM bgGMM(3, numGaussians);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 1");

    CImg<uint8_t> labImg = img.get_RGBtoLab();

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 2");

    cimg_forXY(img, x, y) {
        if (initialFg(x, y)) {
            // foreground
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);
            fgGMM.insertData(color, 1);
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 3");

    // Add border to background GMM
    // top & bottom
    
    // FIXME arbitrary parameter!
    const int inc = 2;

    for (int x = 0; x < img.width(); x += inc) {
        for (int y = 0; y < img.height(); y += img.height() - 1) {
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);

            bgGMM.insertData(color, 1);
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 4");

    for (int y = 0; y < img.height(); y += inc) {
        for (int x = 0; x < img.width(); x += img.width() - 1) {
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);

            bgGMM.insertData(color, 1);
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 5");

    // FIXME arbitrary parameter!
    fgGMM.iterateGMM(50);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 6");

    bgGMM.iterateGMM(50);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 7");

    CImg<float> lFg(img.width(), img.height());
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 8");

    CImg<float> lBg(img.width(), img.height());

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 9");

    CImg<bool> mask(img.width(), img.height());

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 10");

    // Evaluate GMMs
    cimg_forXY(img, x, y) {
        float color[3];
        color[0] = labImg(x, y, 0, 0);
        color[1] = labImg(x, y, 0, 1);
        color[2] = labImg(x, y, 0, 2);
        lFg(x, y) = fgGMM.getLikelihood(color);
        lBg(x, y) = bgGMM.getLikelihood(color);

        mask(x, y) = lFg(x, y) > lBg(x, y);
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 11");

    // Refine FG GMM
    fgGMM.clear();

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 12");
    
    refineGMM(labImg, lFg, lBg, mask, fgGMM, 0.25f);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 13");

    // FIXME arbitrary parameter!
    fgGMM.iterateGMM(50);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 14");

    // Evaluate GMMs again
    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);
            lFg(x, y) = fgGMM.getLikelihood(color);
            lBg(x, y) = bgGMM.getLikelihood(color);

            mask(x, y) = lFg(x, y) > lBg(x, y);
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 15");

    eraseDisconnectedBg(mask);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 16");

    // FIXME 50 is arbitrary!
    count = countConnComponents(mask, 50);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DEBUGGING PART 17");

    outline = CImg<bool>(mask.width(), mask.height()); 

    createOutline(mask, outline);
}
