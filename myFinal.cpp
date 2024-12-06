#include "myCanvas.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "include/GMatrix.h"
#include <cassert>
#include <optional>
#include "include/GBitmap.h"
#include "include/GShader.h"
#include "include/GPath.h"
#include "include/GPathBuilder.h"
//#include "myShader.h"
//#include "myShader.cpp"


//#include "myCanvas.cpp"
#include "myFinal.h"
#include "include/GFinal.h"
/*
MyFinal::myFinal(const GBitmap& bitmap, const GMatrix& localMatrix) {
    fBitmap = bitmap;
    fLocalMatrix = localMatrix;
    gradient = false;
}

MyFinal::myFinal(const GBitmap& bitmap, const GMatrix& localMatrix, GTileMode tilemode1) {
    fBitmap = bitmap;
    fLocalMatrix = localMatrix;
    gradient = false;
    tilemode = tilemode1;
}


MyFinal::myFinal(GPoint p0, GPoint p1, std::vector<GColor> colors, int count) {
    p0_f = p0;
    p1_f = p1;
    color_list_f = colors;
    count_f = count;
    gradient = true;
}

MyFinal::myFinal(GPoint p0, GPoint p1, std::vector<GColor> colors, int count, GTileMode tilemode1) {
    p0_f = p0;
    p1_f = p1;
    color_list_f = colors;
    count_f = count;
    gradient = true;
    tilemode = tilemode1;
    rel_pos = false;
}

MyFinal::myFinal(GPoint p0, GPoint p1, GColor color1, GColor color2) {
    p0_f = p0;
    p1_f = p1;
    std::vector<GColor> list = {color1, color2};
    color_list_f = list;
    count_f = 2;
    gradient = true;
    rel_pos = false;
}

MyFinal::myFinal(GPoint p0, GPoint p1, std::vector<GColor> colors) {
    p0_f = p0;
    p1_f = p1;
    std::vector<GColor> list = {color1, color2};
    color_list_f = list;
    count_f = 2;
    gradient = true;
    rel_pos = false;
}

*/


std::shared_ptr<GShader> MyFinal::createSweepGradient(GPoint center, float startRadians,
                                                         const GColor colors[], int count) 
    {
    
    /*
    std::vector<GColor> colorVector(colors, colors + count);
    return std::make_shared<MyFinal>(center, GPoint(center.x + 1, center.y + 1), colorVector);
    */
    //std::vector<GColor> colorVector(colors, colors + count);
    //return myShader(center, center, colorVector, count);
    return nullptr;
    }
    


std::shared_ptr<GShader> MyFinal::createVoronoiShader(const GPoint points[],
                                                         const GColor colors[],
                                                         int count) 

    {
        /*
    assert(bitmap.width() > 0);
    assert(bitmap.height() > 0);
    GMatrix m = localm;

    GBitmap bm = bitmap;

    
    
    return std::make_shared<myFinal>(bm, m);
    */
   return nullptr;
    }



std::shared_ptr<GShader> MyFinal::createLinearPosGradient(GPoint p0, GPoint p1,
                                                             const GColor colors[],
                                                             const float pos[],
                                                             int count)
     {
        /*
    assert(bitmap.width() > 0);
    assert(bitmap.height() > 0);
    GMatrix m = localm; //* GMatrix::Scale(bitmap.width(), bitmap.height());

    GBitmap bm = bitmap;
    //bm.readFromFile("apps/spock.png");
    
    
    return std::make_shared<myShader>(bm, m);
    //return std::make_shared<myShader>(bm, m);
    */
   return nullptr;
    }

std::shared_ptr<GShader> MyFinal::createColorMatrixShader(const GColorMatrix&,
                                                             GShader* realShader)
    {
        /*
    assert(bitmap.width() > 0);
    assert(bitmap.height() > 0);
    GMatrix m = localm; //* GMatrix::Scale(bitmap.width(), bitmap.height());

    GBitmap bm = bitmap;
    //bm.readFromFile("apps/spock.png");
    
    
    return std::make_shared<myShader>(bm, m);
    //return std::make_shared<myShader>(bm, m);
    */
   return nullptr;
    }
    

std::shared_ptr<GPath> MyFinal::strokePolygon(const GPoint[], int count, float width, bool isClosed) {

}


void MyFinal::drawQuadraticCoons(GCanvas*, const GPoint pts[8], const GPoint tex[4],
                                    int level, const GPaint&) 
    {
        /*
        return new std::shared_ptr<GShader>();
        */

    }
    

