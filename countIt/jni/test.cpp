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

#include <limits>

#include <string>

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
    
    (lFg, lBg, mask).display();

    // '1' indicates forward differences
    CImgList<float> gradient = labImg.get_shared_channel(0)
        .get_gradient("xy", 1);

    for (int unaryFactor = 1; unaryFactor < 255; unaryFactor+=1) {
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

        float maxVal = fmax(lFg.max(), lBg.max()) * unaryFactor;
        // maxVal = fmax(maxVal * unaryFactor, fmax(gradient(0).max(),
                    // gradient(1).max()));

        // TODO remove debugging code
        int expectedNumNodes = img.width() * img.height();
        int numNodes = 0;
        int expectedNumEdges = 
                (img.width() * img.height() * 2 -
                img.width() -
                img.height());
        int numEdges = 0;
        cimg_forXY(img, x, y) {
            int lFgI = (lFg(x, y) * unaryFactor / maxVal) * std::numeric_limits<int>::max();
            int lBgI = (lBg(x, y) * unaryFactor / maxVal) * std::numeric_limits<int>::max();

            int nodeIdx = x + y * img.width();
            // source is FG, sink is BG
            numNodes++;
            g.addNode(nodeIdx, lFgI, lBgI);

            float color[3];
            color[0] = labImg(x, y, 0, 0);
            color[1] = labImg(x, y, 0, 1);
            color[2] = labImg(x, y, 0, 2);

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

                    // FIXME compute edge capacities
                    // float grad = fabs(gradient(n)(x, y));
                    int capacity = -std::numeric_limits<int>::max() / 255; // (grad / maxVal) * ;

                    numEdges++;

                    g.addEdge(nodeIdx, nNodeIdx, capacity, capacity);
                }
            }
        }

        printf("num nodes = %d / %d\n", numNodes, expectedNumNodes);
        printf("num edges = %d / %d\n", numEdges, expectedNumEdges);

        g.initGraph();

        g.computeMaxFlow();

        CImg<bool> postMask(img.width(), img.height());

        postMask = false;

        cimg_forXY(img, x, y) {
            postMask(x, y) = g.isNodeOnSrcSide(x + y * img.width());
        }

        postMask.display();

        // postMask.save(("results/unary_factor_" + std::to_string(unaryFactor) + ".png").c_str());

        // (mask, postMask).display();
    }
}

