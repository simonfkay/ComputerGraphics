/** @file main.cpp
 *  @brief Entry point into our program.
 *  
 *  Welcome to the Great Looking Software Render 
 *  code base (Yes, I needed something with a gl prefix).
 *
 *  This is where you will implement your graphics API.
 *
 *  Compile on the terminal with: 
 *
 *  clang++ -std=c++11 main.cpp -o main
 *
 *  @author Mike Shah, Simon Kay
 *  @bug No known bugs.
 */

// Load our libraries
#include <iostream>

// Some define values
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 320

// C++ Standard Libraries
#include <iostream>
#include <algorithm>

// User libraries
#include "GL.h"
#include "Color.h"
#include "TGA.h"
#include "Maths.h"

// Create a canvas to draw on.
TGA canvas(WINDOW_WIDTH, WINDOW_HEIGHT);

// Implementation of Bresenham's Line Algorithm
// The input to this algorithm is two points and a color
// This algorithm will then modify a canvas (i.e. image)
// filling in the appropriate colors.
void drawLine(Vec2 v0, Vec2 v1, TGA& image, ColorRGB color) {
    bool steep = false;
    if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y)) {
        // If the line is steep we want to transpose the image.
        std::swap(v0.x, v0.y);
        std::swap(v1.x, v1.y);
        steep = true;
    }

    if (v0.x > v1.x) {  // make it left-to-right
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }

    for (int xx = v0.x; xx <= v1.x; ++xx) {
        float tt = (xx - v0.x) / (float)(v1.x - v0.x);
        int yy = v0.y * (1.0f - tt) + v1.y * tt;
        if (steep) {
            canvas.setPixelColor(yy, xx, color);
        } else {
            canvas.setPixelColor(xx, yy, color);
        }
    }
}

// Tells if the given coordinates lie inside of the triangle (or on one of the
// edges) defined by the three given vectors
bool insideOfTriangle(Vec2& pos, Vec2& v0, Vec2& v1, Vec2& v2) {
    // Calculate relative position to v0 of all other vectors
    Vec2 pos0 = pos - v0;
    Vec2 v10 = v1 - v0;
    Vec2 v20 = v2 - v0;

    // Calculate components of the projection of pos onto v0 and v1
    double numerator = (pos0.y * v20.x) - (pos0.x * v20.y);
    double denominator = (v10.y * v20.x) - (v10.x * v20.y);
    double w0 = numerator / denominator;

    double w1 = (pos0.y - (w0 * v10.y)) / v20.y;

    // pos is inside the triangle if:
    //  - w0 >= 0
    //  - w1 >= 0
    //  - w0 + w1 <= 1

    return (w0 >= 0) && (w1 >= 0) && (w0 + w1 <= 1);
}

// Draw a triangle
void triangle(Vec2 v0, Vec2 v1, Vec2 v2, TGA& image, ColorRGB color) {
    if (glFillMode == LINE) {
        drawLine(v0, v1, image, color);
        drawLine(v1, v2, image, color);
        drawLine(v2, v0, image, color);
    } else if (glFillMode == FILL) {
        // Create vectors to represent a bounding box for the requested
        // triangle
        Vec2 tl = v0.min(v1.min(v2));
        Vec2 br = v0.max(v1.max(v2));

        // Color all pixels in the bounding box that lie inside the triangle
        for (unsigned int ii = tl.x; ii < br.x; ++ii) {
            for (unsigned int jj = tl.y; jj < br.y; ++jj) {
                Vec2 pos = Vec2(ii, jj);
                if (insideOfTriangle(pos, v0, v1, v2)) {
                    canvas.setPixelColor(ii, jj, color);
                }
            }
        }
    }
}

// Main
int main() {
    // A sample of color(s) to play with
    ColorRGB red;
    red.r = 255; red.g = 0; red.b = 0;
        
    // Points for our Line
    Vec2 line[2] = {Vec2(0, 0), Vec2(100, 100)};

    // Set the fill mode
    glPolygonMode(FILL);

    // Draw a line
    drawLine(line[0], line[1], canvas, red);

    // Data for our triangle
    Vec2 tri[3] = {Vec2(160, 60),  Vec2(150, 10), Vec2(75, 190)};

    // Draw a triangle
    triangle(tri[0], tri[1], tri[2], canvas, red);

    // Output the final image
    canvas.outputTGAImage("graphics_lab2.ppm");

    return 0;
}
