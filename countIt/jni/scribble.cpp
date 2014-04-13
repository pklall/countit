#include "scribble.h"

#include <functional>
#include <cassert>

using namespace cimg_library;
using namespace std;

float norm(float x, float y) {
    return sqrt(x * x + y * y);
}

void displayUserScribbles(
        const CImg<float>& background,
        const CImg<float>& palette,
        CImg<int>& scribbles,
        function<void()> afterStrokeCallback) {
    assert(scribbles.is_sameXY(background));
    assert(scribbles.depth() == 1);
    assert(scribbles.spectrum() == 1);

    assert(palette.height() == 1);
    assert(palette.depth() == 1);
    assert(palette.is_sameC(background));

    int strokeRadius = 10;
    
    // Height of the toolbar in pixels.
    // The toolbar will contain buttons of equal size allowing
    // the user to select colors from the palette.
    const int TOOLBAR_HEIGHT = 50;

    CImg<float> UI(
            background.width(),
            background.height() + TOOLBAR_HEIGHT,
            1,
            background.spectrum());

    const int numColors = palette.width();

    // Divide the toolbar
    const int paletteColorWidth = UI.width() / (numColors + 1);

    CImgDisplay disp(UI);

    bool drawing = false;

    int curColor = -1;

    int prevMx = 0;
    int prevMy = 0;
    
    while (!disp.is_closed()) {
        int mx = (int) disp.mouse_x() *
            ((float) UI.width() / (float) disp.width());
        int my = (int) disp.mouse_y() *
            ((float) UI.height() / (float) disp.height());

        int pmx = (int) prevMx *
            ((float) UI.width() / (float) disp.width());
        int pmy = (int) prevMy *
            ((float) UI.height() / (float) disp.height());

        if (disp.wheel() > 0) {
            strokeRadius += fmax(1.0, fmin(disp.width(), disp.height()) / 100);
        } else if (disp.wheel() < 0) {
            strokeRadius -= fmax(1.0, fmin(disp.width(), disp.height()) / 100);
        }

        disp.set_wheel();

        // If left button is down
        if (disp.button() & 0x01) {
            if (!drawing && my < TOOLBAR_HEIGHT) {
                // If the mouse is in the toolbar, then change curColor
                curColor = mx / paletteColorWidth - 1;
            } else {
                // Only draw the stroke if we've started drawing already
                if (drawing) {
                    if (curColor == -1) {
                        float dist = norm(pmx - mx, pmy - my);
                        for (int i = 0; i < dist; i += strokeRadius / 4) {
                            int lx = pmx + (mx - pmx) * i / dist;
                            int ly = pmy + (my - pmy) * i / dist;
                            scribbles.draw_circle(
                                    lx, 
                                    ly - TOOLBAR_HEIGHT,
                                    strokeRadius, &curColor);
                        }
                    } else {
                        scribbles.draw_line(
                                pmx, pmy - TOOLBAR_HEIGHT,
                                mx,  my - TOOLBAR_HEIGHT,
                                &curColor);
                    }
                }
                drawing = true;
            }
        } else {
            if (drawing) {
                afterStrokeCallback();
                drawing = false;
            }
        }
        
        // Update the UI image...

        // Draw the palette toolbar

        // Draw a black rectangle for the last button of the toolbar
        // to indicate that it will reset the color
        float color[UI.spectrum()];

        cimg_forC(UI, c) {
            color[c] = 0.0f;
        }

        UI.draw_rectangle(
                0 * paletteColorWidth, 0,
                1 * paletteColorWidth - 1, TOOLBAR_HEIGHT,
                color);

        // Draw rectangles for each color in the palette
        for (int i = 0; i < numColors; i++) {
            cimg_forC(UI, c) {
                color[c] = palette(i, 0, 0, c);
            }

            UI.draw_rectangle(
                    (i + 1) * paletteColorWidth, 0,
                    (i + 2) * paletteColorWidth - 1, TOOLBAR_HEIGHT,
                    color);
        }

        // Draw a white circle to indicate the selected color
        cimg_forC(UI, c) {
            color[c] = 255.0f;
        }

        UI.draw_circle(
                (int) (paletteColorWidth * (1.0 + curColor + 0.5)),
                TOOLBAR_HEIGHT / 2,
                TOOLBAR_HEIGHT / 4,
                color);
        
        // Draw the background image
        UI.draw_image(0, TOOLBAR_HEIGHT, background);

        // Draw existing scribbles
        cimg_forXY(scribbles, x, y) {
            if (scribbles(x, y) >= 0 && scribbles(x, y) < palette.width()) {
                cimg_forC(palette, c) {
                    // Copy over the palette color indicated by the value
                    // of scribbles(x, y)
                    UI(x, y + TOOLBAR_HEIGHT, 0, c) =
                        palette(scribbles(x, y), 0, 0, c);
                }
            }
        }

        // Draw the cursor
        if (my > TOOLBAR_HEIGHT) {
            cimg_forC(UI, c) {
                color[c] = 255.0f;
            }

            UI.draw_circle(mx, my, strokeRadius, color, 1.0f, 0);
        }

        disp.display(UI.get_resize(disp.width(), disp.height()));

        prevMx = disp.mouse_x();
        prevMy = disp.mouse_y();

        disp.wait();

        disp.resize();
    }
}
