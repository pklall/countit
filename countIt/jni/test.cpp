/*
 * Standalone desktop implementation
 */

#include "Eigen/Dense"

#include <string.h>
#include <stdio.h>

#include "CImg.h"

#include "ibfs.h"

#include "GMM.h"

#include "scribble.h"

using namespace std;

using namespace cimg_library;

int main(
        int argc,
        char** argv) {
    CImg<float> img(argv[1]);

    while(img.size() > 1024 * 1024) {
        img.resize_halfXY();
    }

    const int numGaussians = 5;

    GMM fgGMM(3, numGaussians);
    GMM bgGMM(3, numGaussians);

    CImg<float> palette(2, 1, 1, 3);
    palette(0, 0, 0, 0) = 255.0f;
    palette(0, 0, 0, 1) = 0.0f;
    palette(0, 0, 0, 2) = 0.0f;

    palette(1, 0, 0, 0) = 0.0f;
    palette(1, 0, 0, 1) = 255.0f;
    palette(1, 0, 0, 2) = 0.0f;

    CImg<int> scribbles(img.width(), img.height());

    displayUserScribbles(
            img,
            palette,
            scribbles,
            [&]() {
            printf("scribbles done\n");
            });
}

