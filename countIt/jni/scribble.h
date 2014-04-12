#pragma once

#include "CImg.h"

#include <functional>

using namespace cimg_library;
using namespace std;

/**
 * Displays a UI for acquiring "scribbles" from the user.
 *
 * Possible scribble colors are indicated by the colors present
 * in palette, which must be a row of colors.
 *
 * The resulting scribbles are stored as indices into the palette
 * with -1 indicating no scribble.
 */
void displayUserScribbles(
        const CImg<float>& background,
        const CImg<float>& palette,
        CImg<int>& scribbles,
        function<void()> afterStrokeCallback);
