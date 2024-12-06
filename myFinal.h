

#ifndef _g_starter_myFinal_h_
#define _g_starter_myFinal_h_

#include "include/GCanvas.h"
#include "include/GRect.h"
#include "include/GColor.h"
#include "include/GBitmap.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include "include/GMatrix.h"
#include "include/GShader.h"
#include "include/GPathBuilder.h"
#include "include/GFinal.h"
//#include "myCanvas.cpp"

class MyFinal : public GFinal {
public:
        std::shared_ptr<GShader> createSweepGradient(GPoint center, float startRadians,
                                                         const GColor colors[], int count) override;

        std::shared_ptr<GShader> createVoronoiShader(const GPoint points[],
                                                         const GColor colors[],
                                                         int count) override;

        std::shared_ptr<GShader> createLinearPosGradient(GPoint p0, GPoint p1,
                                                             const GColor colors[],
                                                             const float pos[],
                                                             int count) override;

        std::shared_ptr<GShader> createColorMatrixShader(const GColorMatrix&,
                                                             GShader* realShader) override;

        std::shared_ptr<GPath> strokePolygon(const GPoint[], int count, float width, bool isClosed) override;

        void drawQuadraticCoons(GCanvas*, const GPoint pts[8], const GPoint tex[4],
                                    int level, const GPaint&) override;



private:
    GBitmap fBitmap;      // Store the bitmap
    GMatrix fLocalMatrix; // Store the local matrix
    GMatrix fCTM;        // Current transformation matrix
    nonstd::optional<GMatrix> inv;
    GPoint p0_f;
    GPoint p1_f;
    int count_f;
    bool gradient;
    std::vector<GColor> color_list_f;
    GTileMode tilemode;

    bool rel_pos;
    std::vector<GPoint> point_list_f;

    bool bit_avg;
    
};

#endif