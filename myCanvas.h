
/*
 *  Copyright 2024 <me>
 */

#ifndef _g_starter_canvas_h_
#define _g_starter_canvas_h_

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
//#include "myCanvas.cpp"

class MyCanvas : public GCanvas {
public:
    MyCanvas(const GBitmap& device) : fDevice(device) {}

    void clear(const GColor& color) override;
    void drawRect(const GRect& rect, const GPaint& paint) override;
    void drawConvexPolygon(const GPoint[], int count, const GPaint&) override;
    void drawPath(const GPath&, const GPaint&) override;
    void drawQuad(const GPoint verts[4], const GColor colors[4], const GPoint texs[4], int level, const GPaint&) override;
    void drawMesh(const GPoint verts[], const GColor colors[], const GPoint texs[], int count, const int indices[], const GPaint&) override;
    void save() override;
    void restore() override;
    void concat(const GMatrix& matrix) override;
    void blitRow(int x, int y, int count, GPaint paint);
    void drawRectRow(const GRect& rect, const GPaint& paint);
    void drawRect1(const GRect& rect, const GPaint& paint);
    GPixel blend(GPixel src, GPixel* dst, GPaint paint);

    void drawConvexPolygon1(const GPoint[], int count, const GPaint&, const GColor colors[], const GPoint texs[], const GPoint verts[]);
    void blitRow1(int x, int y, int count, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[]);
    GPixel blend1(GPixel src, GPixel* dst, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[], GPoint point);
    void drawRectRow1(const GRect& rect, const GPaint& paint, const GColor colors[], const GPoint texs[], const GPoint verts[]);

    void drawConvexPolygon2(const GPoint[], int count, const GPaint&, const GColor colors[], const GPoint texs[], const GPoint verts[]);
    void blitRow2(int x, int y, int count, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[]);
    GPixel blend2(GPixel src, GPixel* dst, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[], GPoint point);
    void drawRectRow2(const GRect& rect, const GPaint& paint, const GColor colors[], const GPoint texs[], const GPoint verts[]);

    void drawConvexPolygonStroke(const GPoint[], int count, const GPaint&);
    void drawPathStroke(const GPath&, const GPaint&);


    void translate(float tx, float ty);
    void scale(float sx, float sy);
    void rotate(float radians);

private:
    // Note: we store a copy of the bitmap
    const GBitmap fDevice;

    // Add whatever other fields you need
    GMatrix currentMatrix;
    std::stack<GMatrix> matrixStack;
    
};

#endif