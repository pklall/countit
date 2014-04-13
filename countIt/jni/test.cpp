/*
 * Standalone desktop implementation
 */

#include "common.h"

int main(
        int argc,
        char** argv) {
    CImg<float> img(argv[1]);

    while(img.size() > 2048 * 2048) {
        img.resize_halfXY();
    }

    CImg<float> palette(2, 1, 1, 3);
    // Green = foreground
    palette(0, 0, 0, 0) = 0.0f;
    palette(0, 0, 0, 1) = 255.0f;
    palette(0, 0, 0, 2) = 0.0f;

    // Red = background
    palette(1, 0, 0, 0) = 255.0f;
    palette(1, 0, 0, 1) = 0.0f;
    palette(1, 0, 0, 2) = 0.0f;

    CImg<int> scribbles(img.width(), img.height());

    scribbles = -1;

    displayUserScribbles(
            img,
            palette,
            scribbles,
            [&]() {
            });

    CImg<uint8_t> img8 = img;
    CImg<bool> initialFg(img8.width(), img8.height());

    cimg_forXY(scribbles, x, y) {
        if (scribbles(x, y) == 0) {
            initialFg(x, y) = true;
        } else {
            initialFg(x, y) = false;
        }
    }

    CImg<bool> outline(img8.width(), img8.height());

    int count;

    nicename(img8, initialFg, outline, count);

    outline.display();

    printf("count = %d\n", count);

    return 0;
}

    /*

    // const int numGaussians = 5;

    // GMM fgGMM(3, numGaussians);
    // GMM bgGMM(3, numGaussians);

    CImg<float> labImg = img.get_RGBtoLab();

    cimg_forXY(img, x, y) {
        if (scribbles(x, y) == 0) {
            // foreground
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);
            fgGMM.insertData(color, 1);
        } else if (scribbles(x, y) == 1) {
            // background
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);
            bgGMM.insertData(color, 1);
        }
    }

    // Add border to background GMM
    // top & bottom

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

    for (int y = 0; y < img.height(); y += inc) {
        for (int x = 0; x < img.width(); x += img.width() - 1) {
            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);

            bgGMM.insertData(color, 1);
        }
    }

    fgGMM.iterateGMM(50);
    printf("FG model\n");
    fgGMM.printModels();

    bgGMM.iterateGMM(50);
    printf("BG model\n");
    bgGMM.printModels();

    CImg<float> lFg(img.width(), img.height());
    CImg<float> lBg(img.width(), img.height());

    CImg<bool> mask(img.width(), img.height());

    cimg_forXY(img, x, y) {
        float color[3];
        color[0] = labImg(x, y, 0, 0);
        color[1] = labImg(x, y, 0, 1);
        color[2] = labImg(x, y, 0, 2);
        lFg(x, y) = fgGMM.getLikelihood(color);
        lBg(x, y) = bgGMM.getLikelihood(color);

        mask(x, y) = lFg(x, y) > lBg(x, y);
    }
    
    // (lFg, lBg, mask).display();
    // mask.dilate(3);
    // mask.erode(3);
    // mask.erode(3);
    // mask.dilate(3);

    // mask.display();

    // Refine GMM based on current mask

    fgGMM.clear();
    
    refineGMM(labImg, lFg, lBg, mask, fgGMM, 0.25f);

    fgGMM.iterateGMM(50);
    printf("FG model\n");
    fgGMM.printModels();

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

    // mask.display();

    // mask.display();

    // '1' indicates forward differences
    // CImgList<float> gradient = labImg.get_gradient("xy", 1);

    double imgVar[3];

    for (int c = 0; c < 3; c++) {
        imgVar[c] = labImg.get_shared_channel(c).variance();
    }

    // Refine with graph cuts
    if (false) {
        int unaryFactor = 1;
        printf("Processing unary factor %d\n", unaryFactor);

        // Create graph...
        IBFSGraph g;

        g.initSize(
                // number of nodes
                img.width() * img.height(),
                // number of edges
                img.width() * img.height() * 2 -
                img.width() -
                img.height());

        float maxVal = fmax(lFg.max(), lBg.max());
        // maxVal = fmax(maxVal * unaryFactor, fmax(gradient(0).max(),
        // gradient(1).max()));

        cimg_forXY(img, x, y) {
            int lFgI = (lFg(x, y) / maxVal / unaryFactor) * std::numeric_limits<int>::max();
            int lBgI = (lBg(x, y) / maxVal / unaryFactor) * std::numeric_limits<int>::max();

            int nodeIdx = x + y * img.width();
            // source is FG, sink is BG
            g.addNode(nodeIdx, lFgI, lBgI);

            for (int n = 0; n < 2; n++) {
                int nx = x;
                int ny = y;

                if (n == 0) {
                    nx++;
                } else if (n == 1) {
                    ny++;
                }

                if (nx >= 0 && nx < img.width() &&
                        ny >= 0 && ny < img.height()) {
                    int nNodeIdx = nx + ny * img.width();

                    // float grad = gradient(n)(x, y);
                    float grad = 0;

                    for (int c = 0; c < 3; c++) {
                        float diff = labImg(nx, ny, 0, c) - labImg(x, y, 0, c);
                        grad += diff * diff / (2.0f * imgVar[c]);
                    }

                    grad /= 3.0f;

                    grad = exp(-grad);

                    // FIXME
                    int capacity = std::numeric_limits<int>::max() * (grad * 0.002f);

                    g.addEdge(nodeIdx, nNodeIdx, capacity, capacity);
                }
            }
        }

        printf("initializing graph\n");
        g.initGraph();

        printf("computing max flow\n");
        g.computeMaxFlow();

        printf("done\n");

        CImg<bool> postMask(img.width(), img.height());

        postMask = false;

        cimg_forXY(img, x, y) {
            postMask(x, y) = g.isNodeOnSrcSide(x + y * img.width());
        }
    }

    CImg<bool> postMask = mask;

    eraseDisconnectedBg(postMask);

    postMask.display();

    // FIXME 50 is arbitrary!
    int count = countConnComponents(postMask, 50);

    printf("COUNT = %d\n", count);

    CImg<bool> outline(postMask.width(), postMask.height());

    createOutline(postMask, outline);

    outline.display();

    // postMask.save(("results/unary_factor_" + std::to_string(unaryFactor) + ".png").c_str());

    // (mask, postMask).display();
}
    */

