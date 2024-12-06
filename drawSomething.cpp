#include "myCanvas.h"
#include "include/GCanvas.h"
#include "include/GBitmap.h"
#include "include/GColor.h"
#include "include/GPoint.h"
#include "include/GRandom.h"
#include "include/GRect.h"
#include "include/GShader.h"
#include <string>

//#include "apps/image_pa4.cpp"




std::string GDrawSomething(GCanvas* canvas, GISize dim) {


    
    GPathBuilder path;
    //path.moveTo(10,10);
    //path.addCircle({100,100}, 70, GPathDirection::kCCW);
    //path.addCircle({100,120}, 20, GPathDirection::kCW);
    //path.addCircle({85, 75}, 10, GPathDirection::kCW);
    //path.addCircle({115, 85}, 10, GPathDirection::kCW);
    //canvas->drawPath(*path.detach(), GPaint({1,0,0,1}));

    //const GPoint points[] = {(10,10), (100,10), (100, 20), (10,20)};
    
    canvas->scale(2, 2);

    GRect r = GRect::XYWH(10, 15, 100, 75);
    const GColor colors[] = {
        {1,0,0,1}, {0,1,0,1}, {0,0,1,1},
    };
    std::vector<GColor> colors1 = {{1,0,0,1}, {0,1,0,1}, {0,0,1,1}};
    std::vector<GPoint> positions = {{r.left, r.top}, {50,50}, {r.right, r.bottom}};
    auto sh = GCreateLinearGradient({0, 0}, {10,10}, colors, 3);
    GPaint paint(sh);
    canvas->drawRect({-100, -100, 600, 600}, paint);
    

    //canvas->drawConvexPolygon(points, 4, GPaint({1,0,0,1}));
    //draw_grad(canvas);
    
   //draw_tri2(canvas);
   

    
    return "surprised face";
}


