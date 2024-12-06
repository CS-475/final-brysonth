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

#include "include/GFinal.h"
#include "myFinal.h"
//#include "myShader.h"
//#include "myShader.cpp"


/// draw mesh
/// use drawConvexPoly and create shader and use them together

struct Edge {
    float x1, y1; 
    float x2, y2; 
    GPathDirection dir;

    /*
    bool isValid(int y) const {
        return (y >= bottom && y <= top);
    }
    */
};

struct point_dir {
    float i;
    bool up;
};



void MyCanvas::drawMesh(const GPoint verts[], const GColor colors[], const GPoint texs[], int count, const int indices[], const GPaint& paint) {

    
    if (count == 1) {
        if (colors == nullptr && texs == nullptr) { // no tex or colors
            drawConvexPolygon1(verts, 3, paint, colors, texs, verts);
            return;
        }
        else if (colors == nullptr) { // colors (drawtri)
            drawConvexPolygon1(verts, 3, paint, colors, texs, verts);
            return;
        }
        else if (texs == nullptr) { // texs
            //const GColor colors1[] = {GColor({1,0,0,1}), GColor({0,0,1,1}), GColor({0,1,0,1})};
            drawConvexPolygon2(verts, 3, paint, colors, texs, verts);
            return;
        }
    }

    for (int c = 0; c < count * 3; c += 3) {

        const GPoint points11[] = {verts[indices[c]], verts[indices[c+1]], verts[indices[c+2]]};
        GColor color0;
        GColor color1;
        GColor color2;

        GPoint tex0;
        GPoint tex1;
        GPoint tex2;

        if (colors != nullptr) {
            color0 = colors[indices[c]];
            color1 = colors[indices[c+1]];
            color2 = colors[indices[c+2]];
        }

        if (texs != nullptr) {
            tex0 = texs[indices[c]];
            tex1 = texs[indices[c+1]];
            tex2 = texs[indices[c+2]];
        }

        const GColor colors11[] = {color0, color1, color2};
        const GPoint texs11[] = {tex0, tex1, tex2};

        drawConvexPolygon1(points11, 3, paint, colors11, texs11, points11);
    }

}


void MyCanvas::drawQuad(const GPoint verts[4], const GColor colors[4], const GPoint texs[4], int level, const GPaint& paint) {
    if (colors == nullptr) {
        if (level > 0) {
            GPoint mid1 = {(verts[0].x + verts[1].x) / 2, (verts[0].y + verts[1].y) / 2};
            GPoint mid2 = {(verts[1].x + verts[2].x) / 2, (verts[1].y + verts[2].y) / 2};
            GPoint mid3 = {(verts[2].x + verts[3].x) / 2, (verts[2].y + verts[3].y) / 2};
            GPoint mid4 = {(verts[0].x + verts[3].x) / 2, (verts[0].y + verts[3].y) / 2};
            GPoint center = {(mid1.x + mid3.x) / 2, (mid1.y + mid3.y) / 2};

            const GPoint n_verts[] = {verts[0], mid1, center, mid4};
            const GPoint n_verts1[] = {mid1, verts[1], mid2, center};
            const GPoint n_verts2[] = {center, mid2, verts[2], mid3};
            const GPoint n_verts3[] = {mid4, center, mid3, verts[3]};

            if (colors != nullptr) {
                const GColor colors1[] = {colors[0], colors[1], colors[2], colors[3]};
                drawQuad(n_verts, colors1, texs, level - 3, paint);
                drawQuad(n_verts1, colors1, texs, level - 3, paint);
                drawQuad(n_verts2, colors1, texs, level - 3, paint);
                drawQuad(n_verts3, colors1, texs, level - 3, paint);
            }
            else {
                drawQuad(n_verts, colors, texs, level - 12, paint);
                drawQuad(n_verts1, colors, texs, level - 12, paint);
                drawQuad(n_verts2, colors, texs, level - 12, paint);
                drawQuad(n_verts3, colors, texs, level - 12, paint);
            }

        }
        else {
            const GPoint n_verts[] = {verts[0], verts[1], verts[3]};
            const GPoint n_verts1[] = {verts[1], verts[2], verts[3]};

            drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
            drawConvexPolygon1(n_verts1, 3, paint, colors, texs, n_verts1);
        }



    }
    if (colors != nullptr) {
        
        GPoint mid1 = {(verts[0].x + verts[1].x) / 2, (verts[0].y + verts[1].y) / 2};
        GPoint mid2 = {(verts[1].x + verts[2].x) / 2, (verts[1].y + verts[2].y) / 2};
        GPoint mid3 = {(verts[2].x + verts[3].x) / 2, (verts[2].y + verts[3].y) / 2};
        GPoint mid4 = {(verts[0].x + verts[3].x) / 2, (verts[0].y + verts[3].y) / 2};
        GPoint center = {(mid1.x + mid3.x) / 2, (mid1.y + mid3.y) / 2};

        const GPoint n_verts[] = {verts[0], mid1, center, mid4};
        const GPoint n_verts1[] = {mid1, verts[1], mid2, center};
        const GPoint n_verts2[] = {center, mid2, verts[2], mid3};
        const GPoint n_verts3[] = {mid4, center, mid3, verts[3]};

        

       
        if (level > 0) {
            GPoint mid1 = {(verts[0].x + verts[1].x) / 2, (verts[0].y + verts[1].y) / 2};
            GPoint mid2 = {(verts[1].x + verts[2].x) / 2, (verts[1].y + verts[2].y) / 2};
            GPoint mid3 = {(verts[2].x + verts[3].x) / 2, (verts[2].y + verts[3].y) / 2};
            GPoint mid4 = {(verts[0].x + verts[3].x) / 2, (verts[0].y + verts[3].y) / 2};
            GPoint center = {(mid1.x + mid3.x) / 2, (mid1.y + mid3.y) / 2};

            const GPoint n_verts[] = {verts[0], mid1, center, mid4};
            const GPoint n_verts1[] = {mid1, verts[1], mid2, center};
            const GPoint n_verts2[] = {center, mid2, verts[2], mid3};
            const GPoint n_verts3[] = {mid4, center, mid3, verts[3]};

            if (colors != nullptr) {
                const GColor colors1[] = {colors[0], colors[1], colors[2], colors[3]};
                drawQuad(n_verts, colors1, texs, level - 7, paint);
                drawQuad(n_verts1, colors1, texs, level - 7, paint);
                drawQuad(n_verts2, colors1, texs, level - 7, paint);
                drawQuad(n_verts3, colors1, texs, level - 7, paint);
            }
            else {
                drawQuad(n_verts, colors, texs, level - 7, paint);
                drawQuad(n_verts1, colors, texs, level - 7, paint);
                drawQuad(n_verts2, colors, texs, level - 7, paint);
                drawQuad(n_verts3, colors, texs, level - 7, paint);
            }

        } else {
            const GPoint n_verts[] = {verts[0], verts[1], verts[3]};
            const GPoint n_verts1[] = {verts[1], verts[2], verts[3]};


            GColor colors1[] = {colors[0], colors[1], colors[2], colors[3]};

            // 1
            // Triangle 1
            if (round(verts[0].x) == 50 && round(verts[0].y) == 120) {
                colors1[0] = (colors[0] * (.75)) + (colors[1] * (.25));//GColor({0,0,0,1}); // top right
                colors1[1] = (colors[0] * (.75)) + (colors[3] * (.25));//GColor({0,0,0,1}); // bottom left
                colors1[2] = colors[0];///GColor({0,0,0,1}); // top left
                //colors1[3] = GColor({0,0,0,1}); //
                //colors1[1] = 
                //printf("check 1  1\n");
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
                //return;
            }
            // Triangle 2
            if (round(verts[1].x) == 155 && round(verts[1].y) == 112) {
                colors1[0] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] *.75) + (colors[3]* .25))) * .5; // bottom right
                colors1[1] = (colors[0] * (.75)) + (colors[3] * (.25)); // bottom left
                colors1[2] = (colors[0] * (.75)) + (colors[1] * (.25)); // top right
                //printf("check 1  2\n");
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                return;
            }            

            // 2
            // Triangle 1
            if (round(verts[0].x) == 155 && round(verts[0].y) == 112) {
                //colors1[0] = GColor({0,0,0,0});
                colors1[0] = (colors[0] * (.5)) + (colors[1] * (.5));//GColor({0,0,0,1}); // top right
                colors1[1] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] *.75) + (colors[3]* .25))) * .5; // bottom left
                colors1[2] = (colors[0] * (.75)) + (colors[1] * (.25)); // top left

                //printf("check 2  1\n");
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
            }
            // Triangle 2
            if (round(verts[1].x) == 260 && round(verts[1].y) == 103) {
                //colors1[0] = GColor({0,0,0,0});
                colors1[0] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom right
                colors1[1] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom left
                colors1[2] = (colors[0] * (.5)) + (colors[1] * (.5)); // top right
                //printf("check 2  2\n");
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                return;
            }            

            // 5
            // Triangle 1
            if (round(verts[0].x) == 260 && round(verts[0].y) == 103) {
                colors1[0] = (colors[0] * (.25)) + (colors[1] * (.75));//GColor({0,0,0,1}); // top right
                colors1[1] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom left
                colors1[2] = (colors[0] * (.5)) + (colors[1] * (.5)); // top left
                //printf("check 3  1\n");
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
            }
            // Triangle 2
            if (round(verts[1].x) == 365 && round(verts[1].y) == 95) {
                colors1[0] = (((colors[0] * (.25)) + (colors[1] * (.75))) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom right
                colors1[1] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] *.75) + (colors[3]* .25))) * .5; // bottom left
                colors1[2] = (colors[0] * (.25)) + (colors[1] * (.75));// top right

                //printf("check 3  2\n");
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                return;
            }    

            // 6
            // Triangle 1
            if (round(verts[0].x) == 365 && round(verts[0].y) == 95) {
                colors1[0] = colors[1];//GColor({0,0,0,1}); // top right
                colors1[1] = (((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom left
                colors1[2] = (colors[0] * (.25)) + (colors[1] * (.75)); // top left
                //printf("check 3  1\n");
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
            }
            // Triangle 2
            if (round(verts[1].x) == 470 && round(verts[1].y) == 86) {
                colors1[0] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom right
                colors1[1] = (((colors[0] * (.25)) + (colors[1] * (.75))) + ((colors[0] *.75) + (colors[3]* .25))) * .5; // bottom left
                colors1[2] = colors[1];// top right

                //printf("check 3  2\n");
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                return;
            }   
            
            // 3
            // Triangle 2
            if (round(verts[1].x) == 270 && round(verts[1].y) == 175) {
                colors1[0] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom left
                colors1[2] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] * .75) + (colors[3] * .25))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
                colors1[0] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom right
                colors1[1] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom left
                colors1[2] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }
            
            
            
            // 4
            // Triangle 2
            if (round(verts[1].x) == 211 && round(verts[1].y) == 189) {
                colors1[0] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = ((colors[0]) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom left
                colors1[2] = ((colors[0] * .75) + (colors[3] * .25)); // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom right
                colors1[1] = ((colors[0]) + ((colors[0] *.5) + (colors[3]* .5))) * .5;// bottom left
                colors1[2] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }
            
            // 7
            // Triangle 1
            /*
            if (round(verts[0].x) == 328 && round(verts[0].y == 162)) {
                colors1[0] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom left
                colors1[2] = (colors[0] * (.75)) + (colors[3] * (.25)); // top left
                //drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
                drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors, texs, n_verts1);
            }
            */
            // Triangle 2
            if (round(verts[1].x) == 387 && round(verts[1].y) == 148) {
                colors1[0] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//((colors[1] * .75) + (colors[2] * .25)); // top right
                colors1[1] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = GColor({.3,.8,0,1 });//(((colors[0] * .25) + (colors[1] * .75)) + ((colors[1] * (.75)) + (colors[2] * (.25)))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = ((colors[1] * .5) + (colors[2] * .5)); // bottom right
                colors1[1] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//((colors[1] * .75) + (colors[2] * .25)); // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);//
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 8
            // Triangle 1
            /*
            if (round(verts[0].x) == 270 && round(verts[0].y == 175)) {
                colors1[0] = (((colors[0] * .75) + (colors[1] * .25)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] *.75) + (colors[3]* .25))) * .5;// bottom left
                colors1[2] = (colors[0] * (.75)) + (colors[3] * (.25)); // top left
                //drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);
                drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors, texs, n_verts1);
            }
            */
            // Triangle 2
            if (round(verts[1].x) == 328 && round(verts[1].y) == 162) {
                colors1[0] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom left
                colors1[2] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] * .75) + (colors[3] * .25))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * (.75)) + (colors[2] * (.25)))) * .5; // top right ///
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);//
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 9 //
            // Triangle 2
            if (round(verts[1].x) == 291 && round(verts[1].y) == 229) {
                colors1[0] = (colors[1] + ((colors[0] *.25) + (colors[3]* .75))) * .5;//(((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] * .75) + (colors[2] * .25))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * (.75)) + (colors[2] * (.25)))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);//
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 10
            // Triangle 2
            if (round(verts[1].x) == 304 && round(verts[1].y) == 210) {
                colors1[0] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[3] * .5) + (colors[1] * .5)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom left
                colors1[2] = (((colors[0] * .5) + (colors[1] * .5)) + ((colors[0] * .75) + (colors[3] * .25))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * (.75)) + (colors[2] * (.25)))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }
            
            // 11 // bottom row right most ////
            // Triangle 2
            if (round(verts[1].x) == 220 && round(verts[1].y) == 271) {
                colors1[0] = (colors[1] *.25) + (colors[2]* .75); // top right //
                colors1[1] = ((colors[3] *.25) + (colors[2]* .75)); // bottom left //
                colors1[2] = (((colors[2] * .75) + (colors[3] * .25)) + ((colors[1] * .25) + (colors[2] * .75))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = colors[2]; // bottom right //
                colors1[1] = ((colors[3] *.25) + (colors[2]* .75)); // bottom left //
                colors1[2] = (colors[1] *.25) + (colors[2]* .75);// top right //
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 12 // bottom row second to the right
            // Triangle 2
            if (round(verts[1].x) == 254 && round(verts[1].y) == 295) {
                colors1[0] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * .25) + (colors[2] * .75))) * .5; // top right//
                colors1[1] = ((colors[3] * .5) + (colors[2] * .5)); // bottom left //
                colors1[2] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] * .25) + (colors[2] * .75))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = ((colors[3] * .25) + (colors[2] * .75)); // bottom right //
                colors1[1] = ((colors[3] * .5) + (colors[2] * .5)); // bottom left //
                colors1[2] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * .25) + (colors[2] * .75))) * .5; // top right //
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }


            // 13
            // Triangle 2
            if (round(verts[1].x) == 267 && round(verts[1].y) == 266) {
                colors1[0] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[2] * .75) + (colors[1] * .25))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (colors[1] + ((colors[3] *.75) + (colors[2]* .25))) * .5;//(((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * (.75)) + (colors[2] * (.25)))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 14
            // Triangle 2
            if (round(verts[1].x) == 279 && round(verts[1].y) == 248) {
                colors1[0] = (colors[3] + ((colors[3] *.75) + (colors[1]* .25))) * .5;//(((colors[0] * .25) + (colors[1] * .75)) + ((colors[0] * (.75)) + (colors[3] * (.25)))) * .5; // top right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[0] *.5) + (colors[3]* .5))) * .5; // bottom left
                colors1[2] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[0] * .75) + (colors[3] * .25))) * .5; // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom right
                colors1[1] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] *.5) + (colors[2]* .5))) * .5; // bottom left
                colors1[2] = (colors[1] + ((colors[0] *.75) + (colors[3]* .25))) * .5;//(((colors[3] * .25) + (colors[2] * .75)) + ((colors[1] * (.75)) + (colors[2] * (.25)))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 15   bottom row second left most
            // Triangle 2
            if (round(verts[1].x) == 288 && round(verts[1].y) == 320) {
                colors1[0] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] * .25) + (colors[2] * .75))) * .5; // top right
                colors1[1] = ((colors[3] * .75) + (colors[2] * .25)); // bottom left
                colors1[2] =  (((colors[3] * .75) + (colors[2] * .25)) + ((colors[0] *.25) + (colors[3]* .75))) * .5;// top left //
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = (colors[2] * .5) + (colors[3] * .5); // bottom right //
                colors1[1] = ((colors[3] * .75) + (colors[2] * .25)); // bottom left //
                colors1[2] = (((colors[3] * .5) + (colors[2] * .5)) + ((colors[1] * .25) + (colors[2] * .75))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }

            // 16 ///  bottom row left most
            // Triangle 2
            if (round(verts[1].x) == 322 && round(verts[1].y) == 344) {
                colors1[0] = (((colors[3] * .75) + (colors[2] * .25)) + ((colors[0] *.25) + (colors[3]* .75))) * .5; // top right
                colors1[1] = colors[3]; // bottom left
                colors1[2] = ((colors[0] * .25) + (colors[3] * .75)); // top left
                drawConvexPolygon1(n_verts, 3, paint, colors1, texs, n_verts);//
                colors1[0] = ((colors[3] * .75) + (colors[2] * .25)); // bottom right
                colors1[1] = colors[3]; // bottom left
                colors1[2] = (((colors[3] * .75) + (colors[2] * .25)) + ((colors[0] *.25) + (colors[3]* .75))) * .5; // top right
                //drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);
                //drawConvexPolygon1(n_verts, 3, paint, colors, texs, n_verts);
                drawConvexPolygon1(n_verts1, 3, paint, colors1, texs, n_verts1);//
                return;
            }
        }
    }
}


void GPath::ChopCubicAt(const GPoint src[4], GPoint dst[7], float t) {
    GPoint A = { (1 - t) * src[0].x + t * src[1].x, (1 - t) * src[0].y + t * src[1].y };
    GPoint B = { (1 - t) * src[1].x + t * src[2].x, (1 - t) * src[1].y + t * src[2].y };
    GPoint C = { (1 - t) * src[2].x + t * src[3].x, (1 - t) * src[2].y + t * src[3].y };
    GPoint D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
    GPoint E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
    GPoint F = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

    dst[0] = src[0];
    dst[1] = A;
    dst[2] = D;
    dst[3] = F;
    dst[4] = E;
    dst[5] = C;
    dst[6] = src[3];
}


void GPath::ChopQuadAt(const GPoint src[3], GPoint dst[5], float t) {
    GPoint A = { (1 - t) * src[0].x + t * src[1].x, (1 - t) * src[0].y + t * src[1].y };
    GPoint B = { (1 - t) * src[1].x + t * src[2].x, (1 - t) * src[1].y + t * src[2].y };
    GPoint C = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };

    dst[0] = src[0];
    dst[1] = A;
    dst[2] = C;
    dst[3] = B;
    dst[4] = src[2];
}


void GPathBuilder::addCircle(GPoint center, float radius, GPathDirection direction) {
    const int segments = 32;
    const float angleIncrement = 2 * M_PI / segments;

    float firstTheta = 0;
    GPoint firstPoint = { center.x + radius * cos(firstTheta), center.y + radius * sin(firstTheta) };

    moveTo(firstPoint);

    for (int i = 0; i < segments + 1; ++i) {
        float theta1 = i * angleIncrement;
        float theta2 = (i + 1) * angleIncrement;

        GPoint p1 = { center.x + radius * cos(theta1), center.y + radius * sin(theta1) };
        GPoint p2 = { center.x + radius * cos(theta2), center.y + radius * sin(theta2) };
        GPoint p3 = { center.x + radius * cos(-theta1), center.y + radius * sin(-theta1) };
        GPoint p4 = { center.x + radius * cos(-theta2), center.y + radius * sin(-theta2) };

        float controlAngle = (theta1 + theta2) / 2;
        GPoint controlPoint = {
            center.x + (radius * 4 / 3) * cos(controlAngle),
            center.y + (radius * 4 / 3) * sin(controlAngle)
        };

        if (direction == GPathDirection::kCW) {
            this->lineTo(p1);
        } else {
            this->lineTo(p3);
        }
    }
}



void MyCanvas::save() {
    // pushs current matrix onto the stack
    matrixStack.push(currentMatrix);
}

void MyCanvas::restore() {
    
    // Check to see if stack is empty
    if (matrixStack.empty()) {
        return;
    }
     //Restore previous currentMatrix
    currentMatrix = matrixStack.top();
    matrixStack.pop();
    
}

void MyCanvas::concat(const GMatrix& matrix) { 
    // multiply currentMatrix by some matrix and set that equal to currentMatrix
    currentMatrix = GMatrix::Concat(currentMatrix, matrix);
}


void MyCanvas::clear(const GColor& color) {
    int width = fDevice.width();
    int height = fDevice.height();
    unsigned int packedColor = GPixel_PackARGB(
        static_cast<unsigned int>(color.a * 255),
        static_cast<unsigned int>(color.r * 255 * color.a),
        static_cast<unsigned int>(color.g * 255 * color.a),
        static_cast<unsigned int>(color.b * 255 * color.a)
    );

    for (int y = 0; y < height; ++y) {
        GPixel *row_addr = fDevice.getAddr(0, y);
        for (int x = 0; x < width; ++x) {
            row_addr[x] = packedColor;
        }
    }
}

static float dcross(double a, double b, double c, double d) {
    return static_cast<float>(a * b - c * d);
}


void MyCanvas::drawPath(const GPath& path, const GPaint& paint) {
    /*
    GPath::Edger edger(path);
    GPoint pts[GPath::kMaxNextPoints];
    std::vector<GPoint> points;

    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        paint.peekShader()->setContext(ctm);
    }

    nonstd::optional<GPathVerb> verb;
    while ((verb = edger.next(pts)).has_value()) {
        GPathVerb verb1 = verb.value();
        if (verb1 == GPathVerb::kMove) {
            points.push_back(pts[0]);
        }
        else if (verb1 == GPathVerb::kLine) {
            points.push_back(pts[0]);
            points.push_back(pts[1]);
        }
        
        else if (verb1 == GPathVerb::kQuad) {
            
            if (points.empty()) {
                continue;
            }
            

            const GPoint& p0 = pts[0];
            const GPoint& p1 = pts[1];
            const GPoint& p2 = pts[2]; 

            
            float t = .5;

            t = .1;
            
            GPoint A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            GPoint B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C1 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            
            t = .2;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C2 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .3;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C3 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .4;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C4 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .5;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C5 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .6;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C6 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .7;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C7 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .8;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C8 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .9;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C9 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };

            
            points.push_back(p0);
            points.push_back(C1);
            points.push_back(C2);
            points.push_back(C3);
            points.push_back(C4);
            points.push_back(C5);
            points.push_back(C6);
            points.push_back(C7);
            points.push_back(C8);
            points.push_back(C9);
            points.push_back(p2);
            
        }
        
        
    else if (verb1 == GPathVerb::kCubic) {
        const GPoint& p0 = pts[0];
        const GPoint& p1 = pts[1];
        const GPoint& p2 = pts[2];
        const GPoint& p3 = pts[3];

        float t = .1;
        GPoint A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        GPoint B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        GPoint C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        GPoint D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        GPoint E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F1 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .2;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F2 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .3;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F3 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .4;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F4 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .5;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F5 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .6;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F6 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .7;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F7 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .8;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F8 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .9;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F9 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        points.push_back(p0);
        points.push_back(F1);
        points.push_back(F2);
        points.push_back(F3);
        points.push_back(F4);
        points.push_back(F5);
        points.push_back(F6);
        points.push_back(F7);
        points.push_back(F8);
        points.push_back(F9);
        points.push_back(p3);
        
        
    }


        
        else {
            break;
        }
    }

    if (points.empty()) return;

    std::vector<GPoint> mappedPoints(points.size());
    //if (paint.peekShader() != nullptr) {
        currentMatrix.mapPoints(points.data(), points.data(), points.size());
    //}

    int canvasWidth = fDevice.width();
    int canvasHeight = fDevice.height();
    float canvas_width = fDevice.width();
    float canvas_height = fDevice.height();
    std::vector<Edge> edges;

    int count = points.size();



    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;
        
        
        if (points[i].x == 5000) {
            //edges.push_back({points[j].x, points[j].y, points[j].x, points[j].y});
            continue;
        }
        if (points[j].x == 5000) {
            //edges.push_back({points[i].x, points[i].y, points[i].x, points[i].y});
            continue;
        }
        if (points[i].x == 6000) {
            edges.push_back({points[i].x, points[i].y, points[i].x, points[i].y + 1});
            continue;
        }
        if (points[j].x == 6000) {
            //edges.push_back({points[i].x, points[i].y, points[i].x, points[i].y});
            continue;
        }
        
         
        //edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});

        edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});
        
        //float left = points[i].x;
        //float top = points[i].y;
        //float right = points[i].x + 3;
        //float bot = points[i].y + 3;
        //if (bot < canvas_height && right < canvas_width){
            //drawRect({left, top, right, bot}, paint);
        //}
        
        
        edges.push_back({points[j].x, points[j].y, points[j].x + 5, points[j].y});
        edges.push_back({points[j].x+5, points[j].y, points[i].x+5, points[i].y});
        edges.push_back({points[i].x+5, points[i].y, points[i].x, points[i].y});

    }
    
    



    std::vector<std::vector<point_dir>> intersections(static_cast<size_t>(canvasHeight));

    
    for (const auto& edge : edges) {
        float x0 = edge.x1;
        float y0 = edge.y1;
        float x1 = edge.x2;
        float y1 = edge.y2;

        if (y0 == y1) continue; 

        float m = (x1 - x0) / (y1 - y0);
        float b = x0 - (m * y0);

        int yStart = std::max(0, static_cast<int>(std::floor(std::min(y0, y1))));
        int yEnd = std::min(canvasHeight - 1, static_cast<int>(std::floor(std::max(y0, y1))));

        for (int y = yStart; y <= yEnd; y++) {
            float x = (m * (y + 0.5f)) + b; 
            if (x >= 0 && x <= canvasWidth) {
                intersections[y].push_back({x, (y0 < y1)});
            }
        }
    }

  
    for (auto& row : intersections) {
        std::sort(row.begin(), row.end(), [](const point_dir& a, const point_dir& b) {
            return a.i < b.i;
        });
    }


    GRect bounds = path.bounds();
    int top = std::max(0, static_cast<int>(bounds.top));
    int bot = std::min(canvasHeight, static_cast<int>(bounds.bottom));
    int left = std::max(0, static_cast<int>(bounds.left));
    int right = std::min(canvasWidth, static_cast<int>(bounds.right));

    for (int y = 0; y < canvas_height; y++) {
        const auto& row = intersections[y];
        if (row.size() < 2) continue;
        int w = 0;

        for (size_t i = 0; i < row.size() - 1; i += 1) {
            
            //if (row[i].i == 0) {
                //continue;
            //}
            
            //if (row[i].i <= bounds.left) {
                //continue;
            //}
            
           
            if (row[i].up == true) {
                w++;
            }
            else {
                w--;
            }
            if (w == 0) {
                continue;
            }
            
            float x0 = row[i].i;
            float x1;
            if (i+1 < row.size()) {
                x1 = row[i + 1].i;
            }
            else {
                //x1 = canvas_width;
                x1 = x0;
            }

            if (paint.peekShader() != nullptr) {
                //blitRow(static_cast<int>(x0), y, static_cast<int>(x1 - x0), paint);
                if (x0 + 5 > canvas_width) {
                    blitRow(static_cast<int>(x0), y, static_cast<int>(x1-x0), paint);
                }
                else {
                    blitRow(static_cast<int>(x0), y, static_cast<int>(5), paint);
                }
            } else {
                //drawRect1({std::round(x0), float(y), std::round(x1), float(y + 1)}, paint);
                float p0 = x0;
                float p1;
                if (x0 + 5 > canvas_width) {
                    p1 = x0;
                }
                else {
                    p1 = x0 + 5;
                }
                drawRect1({std::round(x0), float(y), std::round(p1), float(y + 1)}, paint);
            }
        }
    }
    */

    
    GPath::Edger edger(path);
    GPoint pts[GPath::kMaxNextPoints];
    std::vector<GPoint> points;

    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        paint.peekShader()->setContext(ctm);
    }

    nonstd::optional<GPathVerb> verb;
    while ((verb = edger.next(pts)).has_value()) {
        GPathVerb verb1 = verb.value();
        if (verb1 == GPathVerb::kMove) {
            points.push_back(pts[0]);
        }
        else if (verb1 == GPathVerb::kLine) {
            points.push_back(pts[0]);
            points.push_back(pts[1]);
        }
        
        else if (verb1 == GPathVerb::kQuad) {
            
            if (points.empty()) {
                continue;
            }
            

            const GPoint& p0 = pts[0];
            const GPoint& p1 = pts[1];
            const GPoint& p2 = pts[2]; 

            
            float t = .5;

            t = .1;
            
            GPoint A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            GPoint B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C1 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            
            t = .2;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C2 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .3;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C3 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .4;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C4 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .5;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C5 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .6;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C6 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .7;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C7 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .8;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C8 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
            t = .9;
            A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
            B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
            GPoint C9 = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };

            
            points.push_back(p0);
            points.push_back(C1);
            points.push_back(C2);
            points.push_back(C3);
            points.push_back(C4);
            points.push_back(C5);
            points.push_back(C6);
            points.push_back(C7);
            points.push_back(C8);
            points.push_back(C9);
            points.push_back(p2);
            
        }
        
        
    else if (verb1 == GPathVerb::kCubic) {
        const GPoint& p0 = pts[0];
        const GPoint& p1 = pts[1];
        const GPoint& p2 = pts[2];
        const GPoint& p3 = pts[3];

        float t = .1;
        GPoint A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        GPoint B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        GPoint C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        GPoint D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        GPoint E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F1 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .2;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F2 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .3;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F3 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .4;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F4 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .5;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F5 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .6;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F6 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .7;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F7 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .8;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F8 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        t = .9;
        A = { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
        B = { (1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y };
        C = { (1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y };
        D = { (1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y };
        E = { (1 - t) * B.x + t * C.x, (1 - t) * B.y + t * C.y };
        GPoint F9 = { (1 - t) * D.x + t * E.x, (1 - t) * D.y + t * E.y };

        points.push_back(p0);
        points.push_back(F1);
        points.push_back(F2);
        points.push_back(F3);
        points.push_back(F4);
        points.push_back(F5);
        points.push_back(F6);
        points.push_back(F7);
        points.push_back(F8);
        points.push_back(F9);
        points.push_back(p3);
        
        
    }


        
        else {
            break;
        }
    }

    if (points.empty()) return;

    std::vector<GPoint> mappedPoints(points.size());
    //if (paint.peekShader() != nullptr) {
        currentMatrix.mapPoints(points.data(), points.data(), points.size());
    //}

    int canvasWidth = fDevice.width();
    int canvasHeight = fDevice.height();
    float canvas_width = fDevice.width();
    float canvas_height = fDevice.height();
    std::vector<Edge> edges;

    int count = points.size();



    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;
        
        
        if (points[i].x == 5000) {
            //edges.push_back({points[j].x, points[j].y, points[j].x, points[j].y});
            continue;
        }
        if (points[j].x == 5000) {
            //edges.push_back({points[i].x, points[i].y, points[i].x, points[i].y});
            continue;
        }
        if (points[i].x == 6000) {
            edges.push_back({points[i].x, points[i].y, points[i].x, points[i].y + 1});
            continue;
        }
        if (points[j].x == 6000) {
            //edges.push_back({points[i].x, points[i].y, points[i].x, points[i].y});
            continue;
        }
        
         
        edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});
    }
    




    std::vector<std::vector<point_dir>> intersections(static_cast<size_t>(canvasHeight));

    
    for (const auto& edge : edges) {
        float x0 = edge.x1;
        float y0 = edge.y1;
        float x1 = edge.x2;
        float y1 = edge.y2;

        if (y0 == y1) continue; 

        float m = (x1 - x0) / (y1 - y0);
        float b = x0 - (m * y0);

        int yStart = std::max(0, static_cast<int>(std::floor(std::min(y0, y1))));
        int yEnd = std::min(canvasHeight - 1, static_cast<int>(std::floor(std::max(y0, y1))));

        for (int y = yStart; y <= yEnd; y++) {
            float x = (m * (y + 0.5f)) + b; 
            if (x >= 0 && x <= canvasWidth) {
                intersections[y].push_back({x, (y0 < y1)});
            }
        }
    }

  
    for (auto& row : intersections) {
        std::sort(row.begin(), row.end(), [](const point_dir& a, const point_dir& b) {
            return a.i < b.i;
        });
    }


    GRect bounds = path.bounds();
    int top = std::max(0, static_cast<int>(bounds.top));
    int bot = std::min(canvasHeight, static_cast<int>(bounds.bottom));
    int left = std::max(0, static_cast<int>(bounds.left));
    int right = std::min(canvasWidth, static_cast<int>(bounds.right));

    for (int y = 0; y < canvas_height; y++) {
        const auto& row = intersections[y];
        if (row.size() < 2) continue;
        int w = 0;

        for (size_t i = 0; i < row.size() - 1; i += 1) {
            
            //if (row[i].i == 0) {
                //continue;
            //}
            
            //if (row[i].i <= bounds.left) {
                //continue;
            //}
            
           
            if (row[i].up == true) {
                w++;
            }
            else {
                w--;
            }
            if (w == 0) {
                continue;
            }
            
            float x0 = row[i].i;
            float x1;
            if (i+1 < row.size()) {
                x1 = row[i + 1].i;
            }
            else {
                //x1 = canvas_width;
                x1 = x0;
            }

            if (paint.peekShader() != nullptr) {
                blitRow(static_cast<int>(x0), y, static_cast<int>(x1 - x0), paint);
            } else {
                //drawRectRow({std::round(x0), float(y), std::round(x1), float(y + 1)}, paint);
                drawRect1({std::round(x0), float(y), std::round(x1), float(y + 1)}, paint);
            }
        }
    }
}





void GPathBuilder::addRect(const GRect& rect, GPathDirection dir) {
        float topLeftX = rect.left;
        float topLeftY = rect.top;
        float topRightX = rect.right;
        float topRightY = rect.top;
        float bottomLeftX = rect.left;
        float bottomLeftY = rect.bottom;
        float bottomRightX = rect.right;
        float bottomRightY = rect.bottom;

        if (dir == GPathDirection::kCW) {
            // Clockwise
            GPathBuilder::moveTo(topLeftX, topLeftY);
            GPathBuilder::lineTo(topRightX, topRightY);
            GPathBuilder::lineTo(bottomRightX, bottomRightY);
            GPathBuilder::lineTo(bottomLeftX, bottomLeftY);
        } else {
            // Counterclockwise
            GPathBuilder::moveTo(topLeftX, topLeftY);
            GPathBuilder::lineTo(bottomLeftX, bottomLeftY);
            GPathBuilder::lineTo(bottomRightX, bottomRightY);
            GPathBuilder::lineTo(topRightX, topRightY);
        }
}

void GPathBuilder::addPolygon(const GPoint pts[], int count) {
        if (count <= 0) {
            return;
        }

        moveTo(pts[0]);

        for (int i = 1; i < count; ++i) {
            lineTo(pts[i]);
        }
}


/*
std::shared_ptr<GPath> GPath::transform(const GMatrix& matrix) const {
    std::vector<GPoint> transformedPts(fPts.size());

    matrix.mapPoints(transformedPts.data(), fPts.data(), fPts.size());

    return std::make_shared<GPath>(transformedPts, fVbs);

}
*/

// change to be tight around the curve





GRect GPath::bounds() const {
    if (fPts.empty()) {
        return GRect::LTRB(0,0,0,0);
    }

    float min_x = fPts[0].x;
    float max_x = fPts[0].x;
    float min_y = fPts[0].y;
    float max_y = fPts[0].y;

    for (int i = 1; i < countPoints(); i++) {
        const GPoint& point = fPts[i];
        if (point.x < min_x) {
            min_x = point.x;
        }
        if (point.x > max_x) {
            max_x = point.x;
        }
        if (point.y < min_y) {
            min_y = point.y;
        }
        if (point.y > max_y) {
            max_y = point.y;
        }
    }

    return GRect::LTRB(min_x, min_y, max_x, max_y); 
}


void MyCanvas::drawRect(const GRect& rect, const GPaint& paint) {

    GRect transformedRect;
    GPixel* addr = nullptr;
    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    if (blend_mode == GBlendMode::kDst) {
        return;
    }

    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    currentMatrix.mapPoints((GPoint*)&transformedRect, (const GPoint*)&rect, 2);

    int i0 = (0 > transformedRect.left) ? 0 : (int)round(transformedRect.left);
    int i1 = (transformedRect.right > fDevice.width()) ? fDevice.width() : (int)round(transformedRect.right);
    int ii0 = (0 > transformedRect.top) ? 0 : (int)round(transformedRect.top);
    int ii1 = (transformedRect.bottom > fDevice.height()) ? fDevice.height() : (int)round(transformedRect.bottom);

    const GPoint points[] = {
    {i0, ii0},    // Top-left
    {i1, ii0},   // Top-right
    {i1, ii1}, // Bottom-right
    {i0, ii1}   // Bottom-left
    };

    if (paint.peekShader() != nullptr) {
        for (int ii = ii0; ii < ii1; ii++) {
            blitRow(i0, ii, int(i1-i0), paint);
        }
        return;
    }


    //if (rect.height() == 1) {
    

    int srcR = (int)(color.r * 255);
    int srcG = (int)(color.g * 255);
    int srcB = (int)(color.b * 255);
    int srcA = (int)(color.a * 255);



    unsigned int outR, outG, outB, outA;


    for (int ii = ii0; ii < ii1; ++ii) {
        for (int i = i0; i < i1; ++i) {
            addr = fDevice.getAddr(i, ii);

            int dstR = GPixel_GetR(*addr);
            int dstG = GPixel_GetG(*addr);
            int dstB = GPixel_GetB(*addr);
            int dstA = GPixel_GetA(*addr);

            switch (paint.getBlendMode()) {
                case GBlendMode::kClear:
                    outR = 0;
                    outG = 0;
                    outB = 0;
                    outA = 0;
                    break;

                case GBlendMode::kSrc:
                    outR = srcR;
                    outG = srcG;
                    outB = srcB;
                    outA = srcA;
                    break;

                case GBlendMode::kDst:
                    outR = dstR;
                    outG = dstG;
                    outB = dstB;
                    outA = dstA;
                    break;

                case GBlendMode::kSrcOver: // S + (1 - Sa)*D
                    /*outR = srcR + (int)((1.0f - color.a) * dstR);
                    outG = srcG + (int)((1.0f - color.a) * dstG);
                    outB = srcB + (int)((1.0f - color.a) * dstB);
                    outA = srcA + (int)((1.0f - color.a) * dstA);
                    break;*/
                    if (color.a < 255) {
                        outR = (color.r*255)*color.a + (dstR * ((255 - (color.a*255)) / 255));
                        outG = (color.g*255)*color.a + (dstG * ((255 - (color.a*255)) / 255));
                        outB = (color.b*255)*color.a + (dstB * ((255 - (color.a*255)) / 255));
                        outA = 255;
                    } else {
                        outR = color.a*color.r*255;
                        outG = color.a*color.g*255;
                        outB = color.a*color.b*255;
                        outA = color.a*255;
                    }
                    break;


                case GBlendMode::kDstOver: // D + (1 - Da)*S
                    outR = dstR + (int)((1.0f - (dstA / 255.0f)) * srcR);
                    outG = dstG + (int)((1.0f - (dstA / 255.0f)) * srcG);
                    outB = dstB + (int)((1.0f - (dstA / 255.0f)) * srcB);
                    outA = dstA + (int)((1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kSrcIn: // Da * S
                    outR = (int)((dstA / 255.0f) * srcR);
                    outG = (int)((dstA / 255.0f) * srcG);
                    outB = (int)((dstA / 255.0f) * srcB);
                    outA = (int)((dstA / 255.0f) * srcA);
                    break;

                case GBlendMode::kDstIn: // Sa * D
                    outR = (int)(color.a * dstR);
                    outG = (int)(color.a * dstG);
                    outB = (int)(color.a * dstB);
                    outA = (int)(color.a * dstA);
                    break;

                case GBlendMode::kSrcOut: // (1 - Da) * S
                    outR = (int)((1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)((1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)((1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)((1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kDstOut: // (1 - Sa) * D
                    outR = (int)((1.0f - color.a) * dstR);
                    outG = (int)((1.0f - color.a) * dstG);
                    outB = (int)((1.0f - color.a) * dstB);
                    outA = (int)((1.0f - color.a) * dstA);
                    break;

                case GBlendMode::kSrcATop: // Da * S + (1 - Sa) * D
                    outR = (int)((dstA / 255.0f) * srcR + (1.0f - color.a) * dstR);
                    outG = (int)((dstA / 255.0f) * srcG + (1.0f - color.a) * dstG);
                    outB = (int)((dstA / 255.0f) * srcB + (1.0f - color.a) * dstB);
                    outA = (int)((dstA / 255.0f) * srcA + (1.0f - color.a) * dstA);
                    break;

                case GBlendMode::kDstATop: // Sa * D + (1 - Da) * S
                    outR = (int)(color.a * dstR + (1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)(color.a * dstG + (1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)(color.a * dstB + (1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)(color.a * dstA + (1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kXor: // (1 - Sa) * D + (1 - Da) * S
                    outR = (int)((1.0f - color.a) * dstR + (1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)((1.0f - color.a) * dstG + (1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)((1.0f - color.a) * dstB + (1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)((1.0f - color.a) * dstA + (1.0f - (dstA / 255.0f)) * srcA);
                    break;
            }
            
            *addr = GPixel_PackARGB(outA, std::min(outR, outA), std::min(outG, outA), std::min(outB,outA));
        }
    }
    
}

void MyCanvas::drawRect1(const GRect& rect, const GPaint& paint) {

    GRect transformedRect;
    GPixel* addr = nullptr;
    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    if (blend_mode == GBlendMode::kDst) {
        return;
    }

    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    //currentMatrix.mapPoints((GPoint*)&transformedRect, (const GPoint*)&rect, 2);

    int i0 = (0 > rect.left) ? 0 : (int)round(rect.left);
    int i1 = (rect.right > fDevice.width()) ? fDevice.width() : (int)round(rect.right);
    int ii0 = (0 > rect.top) ? 0 : (int)round(rect.top);
    int ii1 = (rect.bottom > fDevice.height()) ? fDevice.height() : (int)round(rect.bottom);

    const GPoint points[] = {
    {i0, ii0},    // Top-left
    {i1, ii0},   // Top-right
    {i1, ii1}, // Bottom-right
    {i0, ii1}   // Bottom-left
    };

    
    if (paint.peekShader() != nullptr) {
        for (int ii = ii0; ii < ii1; ii++) {
            blitRow(i0, ii, int(i1-i0), paint);
        }
        return;
    }
    

    //if (rect.height() == 1) {
    

    int srcR = (int)(color.r * 255);
    int srcG = (int)(color.g * 255);
    int srcB = (int)(color.b * 255);
    int srcA = (int)(color.a * 255);



    unsigned int outR, outG, outB, outA;


    for (int ii = ii0; ii < ii1; ++ii) {
        for (int i = i0; i < i1; ++i) {
            addr = fDevice.getAddr(i, ii);

            int dstR = GPixel_GetR(*addr);
            int dstG = GPixel_GetG(*addr);
            int dstB = GPixel_GetB(*addr);
            int dstA = GPixel_GetA(*addr);

            switch (paint.getBlendMode()) {
                case GBlendMode::kClear:
                    outR = 0;
                    outG = 0;
                    outB = 0;
                    outA = 0;
                    break;

                case GBlendMode::kSrc:
                    outR = srcR;
                    outG = srcG;
                    outB = srcB;
                    outA = srcA;
                    break;

                case GBlendMode::kDst:
                    outR = dstR;
                    outG = dstG;
                    outB = dstB;
                    outA = dstA;
                    break;

                case GBlendMode::kSrcOver: // S + (1 - Sa)*D
                    /*outR = srcR + (int)((1.0f - color.a) * dstR);
                    outG = srcG + (int)((1.0f - color.a) * dstG);
                    outB = srcB + (int)((1.0f - color.a) * dstB);
                    outA = srcA + (int)((1.0f - color.a) * dstA);
                    break;*/
                    if (color.a < 255) {
                        outR = (color.r*255)*color.a + (dstR * ((255 - (color.a*255)) / 255));
                        outG = (color.g*255)*color.a + (dstG * ((255 - (color.a*255)) / 255));
                        outB = (color.b*255)*color.a + (dstB * ((255 - (color.a*255)) / 255));
                        outA = 255;
                    } else {
                        outR = color.a*color.r*255;
                        outG = color.a*color.g*255;
                        outB = color.a*color.b*255;
                        outA = color.a*255;
                    }
                    break;


                case GBlendMode::kDstOver: // D + (1 - Da)*S
                    outR = dstR + (int)((1.0f - (dstA / 255.0f)) * srcR);
                    outG = dstG + (int)((1.0f - (dstA / 255.0f)) * srcG);
                    outB = dstB + (int)((1.0f - (dstA / 255.0f)) * srcB);
                    outA = dstA + (int)((1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kSrcIn: // Da * S
                    outR = (int)((dstA / 255.0f) * srcR);
                    outG = (int)((dstA / 255.0f) * srcG);
                    outB = (int)((dstA / 255.0f) * srcB);
                    outA = (int)((dstA / 255.0f) * srcA);
                    break;

                case GBlendMode::kDstIn: // Sa * D
                    outR = (int)(color.a * dstR);
                    outG = (int)(color.a * dstG);
                    outB = (int)(color.a * dstB);
                    outA = (int)(color.a * dstA);
                    break;

                case GBlendMode::kSrcOut: // (1 - Da) * S
                    outR = (int)((1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)((1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)((1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)((1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kDstOut: // (1 - Sa) * D
                    outR = (int)((1.0f - color.a) * dstR);
                    outG = (int)((1.0f - color.a) * dstG);
                    outB = (int)((1.0f - color.a) * dstB);
                    outA = (int)((1.0f - color.a) * dstA);
                    break;

                case GBlendMode::kSrcATop: // Da * S + (1 - Sa) * D
                    outR = (int)((dstA / 255.0f) * srcR + (1.0f - color.a) * dstR);
                    outG = (int)((dstA / 255.0f) * srcG + (1.0f - color.a) * dstG);
                    outB = (int)((dstA / 255.0f) * srcB + (1.0f - color.a) * dstB);
                    outA = (int)((dstA / 255.0f) * srcA + (1.0f - color.a) * dstA);
                    break;

                case GBlendMode::kDstATop: // Sa * D + (1 - Da) * S
                    outR = (int)(color.a * dstR + (1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)(color.a * dstG + (1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)(color.a * dstB + (1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)(color.a * dstA + (1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kXor: // (1 - Sa) * D + (1 - Da) * S
                    outR = (int)((1.0f - color.a) * dstR + (1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)((1.0f - color.a) * dstG + (1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)((1.0f - color.a) * dstB + (1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)((1.0f - color.a) * dstA + (1.0f - (dstA / 255.0f)) * srcA);
                    break;
            }
            
            *addr = GPixel_PackARGB(outA, std::min(outR, outA), std::min(outG, outA), std::min(outB,outA));
        }
    }
    
    
}

void MyCanvas::drawRectRow(const GRect& rect, const GPaint& paint) {
    GRect transformedRect;
    GPixel* addr = nullptr;
    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    if (blend_mode == GBlendMode::kDst) {
        return;
    }

    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    currentMatrix.mapPoints((GPoint*)&transformedRect, (const GPoint*)&rect, 2);

    transformedRect = rect;

    int i0 = (0 > transformedRect.left) ? 0 : (int)round(transformedRect.left);
    int i1 = (transformedRect.right > fDevice.width()) ? fDevice.width() : (int)round(transformedRect.right);
    int ii0 = (0 > transformedRect.top) ? 0 : (int)round(transformedRect.top);
    int ii1 = (transformedRect.bottom > fDevice.height()) ? fDevice.height() : (int)round(transformedRect.bottom);

    const GPoint points[] = {
    {i0, ii0},    // Top-left
    {i1, ii0},   // Top-right
    {i1, ii1}, // Bottom-right
    {i0, ii1}   // Bottom-left
    };

    
    if (paint.peekShader() != nullptr) {
        for (int ii = ii0; ii < ii1; ii++) {
            blitRow(i0, ii, int(i1-i0), paint);
        }
        return;
    }
    

    int srcR = (int)(color.r * 255);
    int srcG = (int)(color.g * 255);
    int srcB = (int)(color.b * 255);
    int srcA = (int)(color.a * 255);



    unsigned int outR, outG, outB, outA;


    for (int ii = ii0; ii < ii1; ++ii) {
        for (int i = i0; i < i1; ++i) {
            addr = fDevice.getAddr(i, ii);

            int dstR = GPixel_GetR(*addr);
            int dstG = GPixel_GetG(*addr);
            int dstB = GPixel_GetB(*addr);
            int dstA = GPixel_GetA(*addr);

            switch (paint.getBlendMode()) {
                case GBlendMode::kClear:
                    outR = 0;
                    outG = 0;
                    outB = 0;
                    outA = 0;
                    break;

                case GBlendMode::kSrc:
                    outR = srcR;
                    outG = srcG;
                    outB = srcB;
                    outA = srcA;
                    break;

                case GBlendMode::kDst:
                    outR = dstR;
                    outG = dstG;
                    outB = dstB;
                    outA = dstA;
                    break;

                case GBlendMode::kSrcOver: // S + (1 - Sa)*D
                    /*outR = srcR + (int)((1.0f - color.a) * dstR);
                    outG = srcG + (int)((1.0f - color.a) * dstG);
                    outB = srcB + (int)((1.0f - color.a) * dstB);
                    outA = srcA + (int)((1.0f - color.a) * dstA);
                    break;*/
                    if (color.a < 255) {
                        outR = (color.r*255)*color.a + (dstR * ((255 - (color.a*255)) / 255));
                        outG = (color.g*255)*color.a + (dstG * ((255 - (color.a*255)) / 255));
                        outB = (color.b*255)*color.a + (dstB * ((255 - (color.a*255)) / 255));
                        outA = 255;
                    } else {
                        outR = color.a*color.r*255;
                        outG = color.a*color.g*255;
                        outB = color.a*color.b*255;
                        outA = color.a*255;
                    }
                    break;


                case GBlendMode::kDstOver: // D + (1 - Da)*S
                    outR = dstR + (int)((1.0f - (dstA / 255.0f)) * srcR);
                    outG = dstG + (int)((1.0f - (dstA / 255.0f)) * srcG);
                    outB = dstB + (int)((1.0f - (dstA / 255.0f)) * srcB);
                    outA = dstA + (int)((1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kSrcIn: // Da * S
                    outR = (int)((dstA / 255.0f) * srcR);
                    outG = (int)((dstA / 255.0f) * srcG);
                    outB = (int)((dstA / 255.0f) * srcB);
                    outA = (int)((dstA / 255.0f) * srcA);
                    break;

                case GBlendMode::kDstIn: // Sa * D
                    outR = (int)(color.a * dstR);
                    outG = (int)(color.a * dstG);
                    outB = (int)(color.a * dstB);
                    outA = (int)(color.a * dstA);
                    break;

                case GBlendMode::kSrcOut: // (1 - Da) * S
                    outR = (int)((1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)((1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)((1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)((1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kDstOut: // (1 - Sa) * D
                    outR = (int)((1.0f - color.a) * dstR);
                    outG = (int)((1.0f - color.a) * dstG);
                    outB = (int)((1.0f - color.a) * dstB);
                    outA = (int)((1.0f - color.a) * dstA);
                    break;

                case GBlendMode::kSrcATop: // Da * S + (1 - Sa) * D
                    outR = (int)((dstA / 255.0f) * srcR + (1.0f - color.a) * dstR);
                    outG = (int)((dstA / 255.0f) * srcG + (1.0f - color.a) * dstG);
                    outB = (int)((dstA / 255.0f) * srcB + (1.0f - color.a) * dstB);
                    outA = (int)((dstA / 255.0f) * srcA + (1.0f - color.a) * dstA);
                    break;

                case GBlendMode::kDstATop: // Sa * D + (1 - Da) * S
                    outR = (int)(color.a * dstR + (1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)(color.a * dstG + (1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)(color.a * dstB + (1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)(color.a * dstA + (1.0f - (dstA / 255.0f)) * srcA);
                    break;

                case GBlendMode::kXor: // (1 - Sa) * D + (1 - Da) * S
                    outR = (int)((1.0f - color.a) * dstR + (1.0f - (dstA / 255.0f)) * srcR);
                    outG = (int)((1.0f - color.a) * dstG + (1.0f - (dstA / 255.0f)) * srcG);
                    outB = (int)((1.0f - color.a) * dstB + (1.0f - (dstA / 255.0f)) * srcB);
                    outA = (int)((1.0f - color.a) * dstA + (1.0f - (dstA / 255.0f)) * srcA);
                    break;
            }
            
            *addr = GPixel_PackARGB(outA, std::min(outR, outA), std::min(outG, outA), std::min(outB,outA));
        }
    }
}


void MyCanvas::drawConvexPolygon(const GPoint points[], int count, const GPaint& paint) {
    //drawConvexPolygonStroke(points, count, paint);
    //return;


    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    GPoint points1[count];

    
    for (int i = 0; i < count; ++i) {
        points1[i] = points[i];
    }
    
    currentMatrix.mapPoints(points1, points, count);

    points = points1;

    if (count < 3) {
        return;
    }

    float canvas_width = fDevice.width();
    float canvas_height = fDevice.height();

    std::vector<Edge> edges;

    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;

        // if i is inside and j is outside (left) 
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, 0, y});

                        edges.push_back({0, y, 0, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (right)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, canvas_width, y});

                        edges.push_back({canvas_width, y, canvas_width, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (top)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, 0});

                        //edges.push_back({x, 0, points[j].x, 0});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (bottom)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, canvas_height});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (bottom) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (top) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is outside (left) and j is inside
        if (points[i].x < 0) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (right) and j is inside
        if (points[i].x > canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is top and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 > canvas_width) {
                            continue;
                        }
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        // if i is top and j is bottom
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = 0;
                        float y2 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        continue;
                    }
                //}
            }
        //}



        // if i is top and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 < 0) {
                            continue;
                        }
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        
        // if i is right and j is right
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({canvas_width, points[i].y, canvas_width, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        



        
        // if i is right and j is bottom
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }
                        edges.push_back({canvas_width, points[i].y, x2, y2});
                        edges.push_back({x2, y2, x1, y1});

                        continue;
                    }
                //}
            //}
        }



        // if i is right and j is left
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = canvas_width;
                        float y1 = (m * x1) + b;

                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({canvas_width, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});


                        continue;
                        
                    //}
                }
            //}
        }



        // if i is right and j is top
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }


                        edges.push_back({0, points[i].y, 0, y2});
                        edges.push_back({0, y2, x1, 0});
                        continue;
                        
                    }
                //}
            //}
        }



        // if i is bottom and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});


                        
                        
                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});




                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is top
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = canvas_height;
                        float y2 = 0;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        
                        continue;
                    }
                //}
            }
        //}



        // if i is left and j is right
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});



                        continue;
                    //}
                }
            //}
        }



        // if i is left and j is bottom
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = canvas_height;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        // if i is left and j is left
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({0, points[i].y, 0, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        // if i is left and j is top
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = 0;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});
                    }
                }
            }
        }
    }
    

    std::vector<std::vector<float>> list_i(static_cast<size_t>(canvas_height));

    for (const auto& edge : edges) {
        float x0 = edge.x1;
        float y0 = edge.y1;
        float x1 = edge.x2;
        float y1 = edge.y2;

        if (y0 == y1) {
            continue;
        }

        float m = (x1 - x0) / (y1 - y0);
        float b = x0 - (m * y0);

        int y_start = std::max(0, static_cast<int>(std::floor(std::min(y0, y1))));
        int y_end = std::min(static_cast<int>(canvas_height) - 1, static_cast<int>(std::floor(std::max(y0, y1))));

        for (int y = y_start; y <= y_end; y++) {
            float x = (m * (y+.5)) + b;
            if (x >= 0 && x <= canvas_width) {
                list_i[y].push_back(x);
            }
        }
    }

    for (auto& intersections : list_i) {
        std::sort(intersections.begin(), intersections.end());
    }

    for (int y = 0; y < canvas_height; y++) {
        const auto& intersections = list_i[y];
        if (intersections.size() < 2) {
            continue;
        }
     
        for (int i = 0; i < intersections.size(); i += 2) {
            float x0 = intersections[i];
            float x1 = intersections[i + 1];
            if (x0 > x1) {
                std::swap(x0, x1);
            }
            if (paint.peekShader() != nullptr) {
                blitRow(int(x0), y, int(x1-x0), paint); // spock
            }
            else{
                drawRectRow({round(x0), float(y), round(x1), float(y+1)}, paint); // color clock
            }
        }
        
    }
}



GMatrix::GMatrix() : GMatrix(1, 0, 0,
                             0, 1, 0) {}

// Translation matrix
GMatrix GMatrix::Translate(float tx, float ty) {
    return GMatrix(1, 0, tx, 0, 1, ty);
}

void MyCanvas::translate(float tx, float ty) {
    GMatrix translationMatrix = GMatrix::Translate(tx, ty);
    currentMatrix = GMatrix::Concat(currentMatrix, translationMatrix);
}

// Scaling matrix
GMatrix GMatrix::Scale(float sx, float sy) {
    return GMatrix(sx, 0, 0, 0, sy, 0);
}

void MyCanvas::scale(float sx, float sy) {
    GMatrix scalingMatrix = GMatrix::Scale(sx, sy);
    currentMatrix = GMatrix::Concat(currentMatrix, scalingMatrix);
}

// Rotation matrix
GMatrix GMatrix::Rotate(float radians) {
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);

    
    return GMatrix(cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0);
}

void MyCanvas::rotate(float radians) {
    GMatrix rotationMatrix = GMatrix::Rotate(radians);
    currentMatrix = GMatrix::Concat(currentMatrix, rotationMatrix);
}

GMatrix GMatrix::Concat(const GMatrix& A, const GMatrix& B) {
    return GMatrix(A[0] * B[0] + A[2] * B[1],
                   A[0] * B[2] + A[2] * B[3],
                   A[0] * B[4] + A[2] * B[5] + A[4],
                   A[1] * B[0] + A[3] * B[1],
                   A[1] * B[2] + A[3] * B[3],
                   A[1] * B[4] + A[3] * B[5] + A[5]);
}

nonstd::optional<GMatrix> GMatrix::invert() const {
    float det = fMat[0] * fMat[3] - fMat[1] * fMat[2];
    if (det == 0) {
        return {};
    }

    float invDet = 1.0f / det;
    // to do: verify output is inverse
    
    return GMatrix(
        fMat[3] * invDet,  // a'
        -fMat[2] * invDet, // c'
        -(fMat[3] * fMat[4] - fMat[2] * fMat[5]) * invDet, // e'
        -fMat[1] * invDet, // b
        fMat[0] * invDet,  // d'
        -(fMat[0] * fMat[5] - fMat[1] * fMat[4]) * invDet  // f'
    );
}

void GMatrix::mapPoints(GPoint dst[], const GPoint src[], int count) const {
    const auto e0 = this->e0();
    const auto e1 = this->e1();
    const auto origin = this->origin();
    for (int i = 0; i < count; ++i) {
        dst[i] = e0 * src[i].x + e1 * src[i].y + origin;
    }
}

// shaderow and dst are required

void MyCanvas::blitRow(int x, int y, int count, GPaint paint) {
    std::vector<GPixel> storage(count);
    GShader * s = paint.peekShader();
    if (s != nullptr) {
        s ->shadeRow(x,y,count, storage.data()); // red means it doesn't get into shaderow and white means shaderow is incorrect
    }
    else {// shaderow doesn't work right
        std::fill(storage.begin(), storage.end(), GPixel_PackARGB(200, 200, 0, 0));
    }
    for (int i = 0; i < count; i++) {
        GPixel* dst = fDevice.getAddr(x+i, y);
        if (dst != nullptr) {  // Check if the destination address is valid
            GPixel src = storage[i];
            *dst = blend(src, dst, paint);
        }
        else {
            GPixel src = storage[i];
            *dst = src;
        }
    }
    
}


inline int clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}


GPixel MyCanvas::blend(GPixel src, GPixel* dst, GPaint paint) {
    int dstR = GPixel_GetR(*dst);
    int dstG = GPixel_GetG(*dst);
    int dstB = GPixel_GetB(*dst);
    int dstA = GPixel_GetA(*dst);

    int srcR = GPixel_GetR(src);
    int srcG = GPixel_GetG(src);
    int srcB = GPixel_GetB(src);
    int srcA = GPixel_GetA(src);

    int outR, outG, outB, outA;

    switch (paint.getBlendMode()) {
        case GBlendMode::kClear:
            return GPixel_PackARGB(0, 0, 0, 0);

        case GBlendMode::kSrc:
            return src;

        case GBlendMode::kDst:
            return *dst;

        case GBlendMode::kSrcOver: // S + (1 - Sa) * D
            outR = (srcR * srcA + dstR * (255 - srcA)) / 255;
            outG = (srcG * srcA + dstG * (255 - srcA)) / 255;
            outB = (srcB * srcA + dstB * (255 - srcA)) / 255;
            outA = clamp(dstA + srcA, 0, 255);
            break;

        case GBlendMode::kDstOver: // D + (1 - Da) * S
            outR = (dstR * dstA + srcR * (255 - dstA)) / 255;
            outG = (dstG * dstA + srcG * (255 - dstA)) / 255;
            outB = (dstB * dstA + srcB * (255 - dstA)) / 255;
            outA = clamp(dstA + srcA, 0, 255);
            break;

        case GBlendMode::kSrcIn: // Da * S
            outR = (srcR * dstA) / 255;
            outG = (srcG * dstA) / 255;
            outB = (srcB * dstA) / 255;
            outA = (srcA * dstA) / 255;
            break;

        case GBlendMode::kDstIn: // Sa * D
            outR = (dstR * srcA) / 255;
            outG = (dstG * srcA) / 255;
            outB = (dstB * srcA) / 255;
            outA = (dstA * srcA) / 255;
            break;

        case GBlendMode::kSrcOut: // (1 - Da) * S
            outR = (srcR * (255 - dstA)) / 255;
            outG = (srcG * (255 - dstA)) / 255;
            outB = (srcB * (255 - dstA)) / 255;
            outA = (srcA * (255 - dstA)) / 255;
            break;

        case GBlendMode::kDstOut: // (1 - Sa) * D
            outR = (dstR * (255 - srcA)) / 255;
            outG = (dstG * (255 - srcA)) / 255;
            outB = (dstB * (255 - srcA)) / 255;
            outA = (dstA * (255 - srcA)) / 255;
            break;

        case GBlendMode::kSrcATop: // Da * S + (1 - Sa) * D
            outR = (dstA * srcR + (255 - srcA) * dstR) / 255;
            outG = (dstA * srcG + (255 - srcA) * dstG) / 255;
            outB = (dstA * srcB + (255 - srcA) * dstB) / 255;
            outA = clamp(dstA + (srcA * (255 - dstA) / 255), 0, 255);
            break;

        case GBlendMode::kDstATop: // Sa * D + (1 - Da) * S
            outR = (srcA * dstR + (255 - dstA) * srcR) / 255;
            outG = (srcA * dstG + (255 - dstA) * srcG) / 255;
            outB = (srcA * dstB + (255 - dstA) * srcB) / 255;
            outA = clamp(srcA + (dstA * (255 - srcA) / 255), 0, 255);
            break;

        case GBlendMode::kXor: // (1 - Sa) * D + (1 - Da) * S
            outR = ((255 - srcA) * dstR + (255 - dstA) * srcR) / 255;
            outG = ((255 - srcA) * dstG + (255 - dstA) * srcG) / 255;
            outB = ((255 - srcA) * dstB + (255 - dstA) * srcB) / 255;
            outA = clamp((255 - srcA) * dstA + (255 - dstA) * srcA, 0, 255);
            break;
    }

    return GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
}



void MyCanvas::drawConvexPolygon1(const GPoint points[], int count, const GPaint& paint, const GColor colors[], const GPoint texs[], const GPoint verts[]) {
    //drawConvexPolygonStroke(points, count, paint);
    //return;


    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    GPoint points1[count];

    
    for (int i = 0; i < count; ++i) {
        points1[i] = points[i];
    }
    
    currentMatrix.mapPoints(points1, points, count);

    points = points1;

    if (count < 3) {
        return;
    }

    float canvas_width = fDevice.width();
    float canvas_height = fDevice.height();

    std::vector<Edge> edges;

    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;

        // if i is inside and j is outside (left) 
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, 0, y});

                        edges.push_back({0, y, 0, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (right)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, canvas_width, y});

                        edges.push_back({canvas_width, y, canvas_width, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (top)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, 0});

                        //edges.push_back({x, 0, points[j].x, 0});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (bottom)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, canvas_height});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (bottom) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (top) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is outside (left) and j is inside
        if (points[i].x < 0) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (right) and j is inside
        if (points[i].x > canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is top and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 > canvas_width) {
                            continue;
                        }
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        // if i is top and j is bottom
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = 0;
                        float y2 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        continue;
                    }
                //}
            }
        //}



        // if i is top and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 < 0) {
                            continue;
                        }
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        
        // if i is right and j is right
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({canvas_width, points[i].y, canvas_width, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        



        
        // if i is right and j is bottom
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }
                        edges.push_back({canvas_width, points[i].y, x2, y2});
                        edges.push_back({x2, y2, x1, y1});

                        continue;
                    }
                //}
            //}
        }



        // if i is right and j is left
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = canvas_width;
                        float y1 = (m * x1) + b;

                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({canvas_width, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});


                        continue;
                        
                    //}
                }
            //}
        }



        // if i is right and j is top
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }


                        edges.push_back({0, points[i].y, 0, y2});
                        edges.push_back({0, y2, x1, 0});
                        continue;
                        
                    }
                //}
            //}
        }



        // if i is bottom and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});


                        
                        
                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});




                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is top
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = canvas_height;
                        float y2 = 0;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        
                        continue;
                    }
                //}
            }
        //}



        // if i is left and j is right
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});



                        continue;
                    //}
                }
            //}
        }



        // if i is left and j is bottom
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = canvas_height;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        // if i is left and j is left
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({0, points[i].y, 0, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        // if i is left and j is top
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = 0;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});
                    }
                }
            }
        }
    }
    

    std::vector<std::vector<float>> list_i(static_cast<size_t>(canvas_height));

    for (const auto& edge : edges) {
        float x0 = edge.x1;
        float y0 = edge.y1;
        float x1 = edge.x2;
        float y1 = edge.y2;

        if (y0 == y1) {
            continue;
        }

        float m = (x1 - x0) / (y1 - y0);
        float b = x0 - (m * y0);

        int y_start = std::max(0, static_cast<int>(std::floor(std::min(y0, y1))));
        int y_end = std::min(static_cast<int>(canvas_height) - 1, static_cast<int>(std::floor(std::max(y0, y1))));

        for (int y = y_start; y <= y_end; y++) {
            float x = (m * (y+.5)) + b;
            if (x >= 0 && x <= canvas_width) {
                list_i[y].push_back(x);
            }
        }
    }

    for (auto& intersections : list_i) {
        std::sort(intersections.begin(), intersections.end());
    }

    for (int y = 0; y < canvas_height; y++) {
        const auto& intersections = list_i[y];
        if (intersections.size() < 2) {
            continue;
        }
     
        for (int i = 0; i < intersections.size(); i += 2) {
            float x0 = intersections[i];
            float x1 = intersections[i + 1];
            if (x0 > x1) {
                std::swap(x0, x1);
            }
            if (paint.peekShader() != nullptr) {
                blitRow1(int(x0), y, int(x1-x0), paint, colors, texs, verts); // spock
                //drawRectRow1({round(x0), float(y), round(x1), float(y+1)}, paint, colors, texs, verts);
            }
            else{
                drawRectRow1({round(x0), float(y), round(x1), float(y+1)}, paint, colors, texs, verts); // drawtri
            }
        }
        
    }
}


void MyCanvas::blitRow1(int x, int y, int count, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[]) {
    std::vector<GPixel> storage(count);
    GShader * s = paint.peekShader();
    if (s != nullptr) {
        s ->shadeRow(x,y,count, storage.data()); // red means it doesn't get into shaderow and white means shaderow is incorrect
    }
    else {// shaderow doesn't work right
        std::fill(storage.begin(), storage.end(), GPixel_PackARGB(200, 200, 0, 0));
    }
    for (int i = 0; i < count; i++) {
        int xi = x+i;
        if (xi >= fDevice.width()) {
            xi = fDevice.width()-1;
        }
        if (xi < 0) {
            xi = 0;
        }
        GPixel* dst = fDevice.getAddr(xi, y);
        if (dst != nullptr) {  // Check if the destination address is valid
            GPixel src = storage[i];
            GPoint point = GPoint({x+i, y});
            *dst = blend1(src, dst, paint, colors, texs, verts, point);
        }
        else {
            GPixel src = storage[i];
            *dst = src;
        }
    }
    
}


GPixel MyCanvas::blend1(GPixel src, GPixel* dst, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[], GPoint point) {
    int dstR = GPixel_GetR(*dst);
    int dstG = GPixel_GetG(*dst);
    int dstB = GPixel_GetB(*dst);
    int dstA = GPixel_GetA(*dst);

    int srcR = GPixel_GetR(src);
    int srcG = GPixel_GetG(src);
    int srcB = GPixel_GetB(src);
    int srcA = GPixel_GetA(src);

    int outR, outG, outB, outA;

    GPixel tex_pixel;
    GPixel color_pixel;

    if (colors != nullptr) {
        float x0 = verts[1].x;
        float y0 = verts[1].y;
        float x1 = verts[2].x;
        float y1 = verts[2].y;
        float x2 = point.x;
        float y2 = point.y;
        float V0area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        x0 = verts[0].x;
        y0 = verts[0].y;
        x1 = verts[2].x;
        y1 = verts[2].y;
        float V1area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        x0 = verts[0].x;
        y0 = verts[0].y;
        x1 = verts[1].x;
        y1 = verts[1].y;
        float V2area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        float V0w = V0area / (V0area + V1area + V2area);
        float V1w = V1area / (V0area + V1area + V2area);
        float V2w = V2area / (V0area + V1area + V2area);
        GColor src = (colors[0] * V0w) + (colors[1] * V1w) + (colors[2] * V2w);
        srcR = (int)(src.r * 255);
        srcG = (int)(src.g * 255);
        srcB = (int)(src.b * 255);
        srcA = (int)(src.a * 255);
        outR = srcR;
        outG = srcB;
        outB = srcB;
        outA = srcA;
        if (outR > outA) {
            outR = outA;
        }
        if (outG > outA) {
            outG = outA;
        }
        if (outB > outA) {
            outB = outA;
        }
        color_pixel = GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
        if (texs == nullptr) {
            return GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
        }

    }

    
    switch (paint.getBlendMode()) {
        case GBlendMode::kClear:
            return GPixel_PackARGB(0, 0, 0, 0);

        case GBlendMode::kSrc:
            return src;

        case GBlendMode::kDst:
            return *dst;

        case GBlendMode::kSrcOver: // S + (1 - Sa) * D
            outR = (srcR * srcA + dstR * (255 - srcA)) / 255;
            outG = (srcG * srcA + dstG * (255 - srcA)) / 255;
            outB = (srcB * srcA + dstB * (255 - srcA)) / 255;
            outA = clamp(dstA + srcA, 0, 255);
            break;

        case GBlendMode::kDstOver: // D + (1 - Da) * S
            outR = (dstR * dstA + srcR * (255 - dstA)) / 255;
            outG = (dstG * dstA + srcG * (255 - dstA)) / 255;
            outB = (dstB * dstA + srcB * (255 - dstA)) / 255;
            outA = clamp(dstA + srcA, 0, 255);
            break;

        case GBlendMode::kSrcIn: // Da * S
            outR = (srcR * dstA) / 255;
            outG = (srcG * dstA) / 255;
            outB = (srcB * dstA) / 255;
            outA = (srcA * dstA) / 255;
            break;

        case GBlendMode::kDstIn: // Sa * D
            outR = (dstR * srcA) / 255;
            outG = (dstG * srcA) / 255;
            outB = (dstB * srcA) / 255;
            outA = (dstA * srcA) / 255;
            break;

        case GBlendMode::kSrcOut: // (1 - Da) * S
            outR = (srcR * (255 - dstA)) / 255;
            outG = (srcG * (255 - dstA)) / 255;
            outB = (srcB * (255 - dstA)) / 255;
            outA = (srcA * (255 - dstA)) / 255;
            break;

        case GBlendMode::kDstOut: // (1 - Sa) * D
            outR = (dstR * (255 - srcA)) / 255;
            outG = (dstG * (255 - srcA)) / 255;
            outB = (dstB * (255 - srcA)) / 255;
            outA = (dstA * (255 - srcA)) / 255;
            break;

        case GBlendMode::kSrcATop: // Da * S + (1 - Sa) * D
            outR = (dstA * srcR + (255 - srcA) * dstR) / 255;
            outG = (dstA * srcG + (255 - srcA) * dstG) / 255;
            outB = (dstA * srcB + (255 - srcA) * dstB) / 255;
            outA = clamp(dstA + (srcA * (255 - dstA) / 255), 0, 255);
            break;

        case GBlendMode::kDstATop: // Sa * D + (1 - Da) * S
            outR = (srcA * dstR + (255 - dstA) * srcR) / 255;
            outG = (srcA * dstG + (255 - dstA) * srcG) / 255;
            outB = (srcA * dstB + (255 - dstA) * srcB) / 255;
            outA = clamp(srcA + (dstA * (255 - srcA) / 255), 0, 255);
            break;

        case GBlendMode::kXor: // (1 - Sa) * D + (1 - Da) * S
            outR = ((255 - srcA) * dstR + (255 - dstA) * srcR) / 255;
            outG = ((255 - srcA) * dstG + (255 - dstA) * srcG) / 255;
            outB = ((255 - srcA) * dstB + (255 - dstA) * srcB) / 255;
            outA = clamp((255 - srcA) * dstA + (255 - dstA) * srcA, 0, 255);
            break;
    }

    /*
    GPixel t0 = *fDevice.getAddr(texs[0].x, texs[0].y);
    GPixel t1 = *fDevice.getAddr(texs[1].x, texs[1].y);
    GPixel t2 = *fDevice.getAddr(texs[2].x, texs[2].y);
    GPixel t3 = *fDevice.getAddr(texs[3].x, texs[3].y);
    */
    
    


    if (outR > outA) {
        outR = outA;
    }
    if (outG > outA) {
        outG = outA;
    }
    if (outB > outA) {
        outB = outA;
    }

    //return GPixel_PackARGB(0,200,0,220);
    //outR = outR * .6;
    //outG = outG * .6;
    //outB = outB * .6;

    
    return GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
}



void MyCanvas::drawRectRow1(const GRect& rect, const GPaint& paint, const GColor colors[], const GPoint texs[], const GPoint verts[]) {
    GRect transformedRect;
    GPixel* addr = nullptr;
    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    if (blend_mode == GBlendMode::kDst) {
        return;
    }

    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    currentMatrix.mapPoints((GPoint*)&transformedRect, (const GPoint*)&rect, 2);

    transformedRect = rect;

    int i0 = (0 > transformedRect.left) ? 0 : (int)round(transformedRect.left);
    int i1 = (transformedRect.right > fDevice.width()) ? fDevice.width() : (int)round(transformedRect.right);
    int ii0 = (0 > transformedRect.top) ? 0 : (int)round(transformedRect.top);
    int ii1 = (transformedRect.bottom > fDevice.height()) ? fDevice.height() : (int)round(transformedRect.bottom);

    const GPoint points[] = {
    {i0, ii0},    // Top-left
    {i1, ii0},   // Top-right
    {i1, ii1}, // Bottom-right
    {i0, ii1}   // Bottom-left
    };

    
    if (paint.peekShader() != nullptr) {
        for (int ii = ii0; ii < ii1; ii++) {
            blitRow(i0, ii, int(i1-i0), paint);
        }
        return;
    }
    

    int srcR = (int)(color.r * 255);
    int srcG = (int)(color.g * 255);
    int srcB = (int)(color.b * 255);
    int srcA = (int)(color.a * 255);



    unsigned int outR, outG, outB, outA;


    for (int ii = ii0; ii < ii1; ++ii) {
        for (int i = i0; i < i1; ++i) {
            float x0 = verts[1].x;
            float y0 = verts[1].y;
            float x1 = verts[2].x;
            float y1 = verts[2].y;
            float x2 = i;
            float y2 = ii;
            float V0area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
            x0 = verts[0].x;
            y0 = verts[0].y;
            x1 = verts[2].x;
            y1 = verts[2].y;
            float V1area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
            x0 = verts[0].x;
            y0 = verts[0].y;
            x1 = verts[1].x;
            y1 = verts[1].y;
            float V2area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
            float V0w = V0area / (V0area + V1area + V2area);
            float V1w = V1area / (V0area + V1area + V2area);
            float V2w = V2area / (V0area + V1area + V2area);
            GColor src = (colors[2] * V0w) + (colors[0] * V1w) + (colors[1] * V2w);
            srcR = (int)(src.r * 255);
            srcG = (int)(src.g * 255);
            srcB = (int)(src.b * 255);
            srcA = (int)(src.a * 255);

            if (i >= fDevice.width()) {
                i = fDevice.width() - 1;
            }
            if (i < 0) {
                i = 0;
            }
            addr = fDevice.getAddr(i, ii);

            int dstR = GPixel_GetR(*addr);
            int dstG = GPixel_GetG(*addr);
            int dstB = GPixel_GetB(*addr);
            int dstA = GPixel_GetA(*addr);


            outR = srcR;
            outG = srcG;
            outB = srcB;
            outA = srcA;

           if (outA > 255) {
            outA = 255;
           }
            
            
            *addr = GPixel_PackARGB(outA, std::min(outR, outA), std::min(outG, outA), std::min(outB,outA));
        }
    }
}



void MyCanvas::drawConvexPolygon2(const GPoint points[], int count, const GPaint& paint, const GColor colors[], const GPoint texs[], const GPoint verts[]) {
    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    GPoint points1[count];

    
    for (int i = 0; i < count; ++i) {
        points1[i] = points[i];
    }
    
    currentMatrix.mapPoints(points1, points, count);

    //printf("%f, %f\n", points1[0].x, points1[0].y);
    //printf("%f, %f\n", points1[1].x, points1[1].y);
    //printf("%f, %f\n", points1[2].x, points1[2].y);

    points = points1;

    if (count < 3) {
        return;
    }

    float canvas_width = fDevice.width();
    float canvas_height = fDevice.height();

    std::vector<Edge> edges;

    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;

        // if i is inside and j is outside (left) 
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, 0, y});

                        edges.push_back({0, y, 0, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (right)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, canvas_width, y});

                        edges.push_back({canvas_width, y, canvas_width, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (top)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, 0});

                        //edges.push_back({x, 0, points[j].x, 0});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (bottom)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, canvas_height});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (bottom) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (top) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is outside (left) and j is inside
        if (points[i].x < 0) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (right) and j is inside
        if (points[i].x > canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is top and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 > canvas_width) {
                            continue;
                        }
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        // if i is top and j is bottom
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = 0;
                        float y2 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        continue;
                    }
                //}
            }
        //}



        // if i is top and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 < 0) {
                            continue;
                        }
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        
        // if i is right and j is right
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({canvas_width, points[i].y, canvas_width, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        



        
        // if i is right and j is bottom
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }
                        edges.push_back({canvas_width, points[i].y, x2, y2});
                        edges.push_back({x2, y2, x1, y1});

                        continue;
                    }
                //}
            //}
        }



        // if i is right and j is left
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = canvas_width;
                        float y1 = (m * x1) + b;

                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({canvas_width, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});


                        continue;
                        
                    //}
                }
            //}
        }



        // if i is right and j is top
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }


                        edges.push_back({0, points[i].y, 0, y2});
                        edges.push_back({0, y2, x1, 0});
                        continue;
                        
                    }
                //}
            //}
        }



        // if i is bottom and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});


                        
                        
                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});




                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is top
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = canvas_height;
                        float y2 = 0;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        
                        continue;
                    }
                //}
            }
        //}



        // if i is left and j is right
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});



                        continue;
                    //}
                }
            //}
        }



        // if i is left and j is bottom
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = canvas_height;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        // if i is left and j is left
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({0, points[i].y, 0, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        // if i is left and j is top
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = 0;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});
                    }
                }
            }
        }
    }
    

    std::vector<std::vector<float>> list_i(static_cast<size_t>(canvas_height));

    for (const auto& edge : edges) {
        float x0 = edge.x1;
        float y0 = edge.y1;
        float x1 = edge.x2;
        float y1 = edge.y2;

        if (y0 == y1) {
            continue;
        }

        float m = (x1 - x0) / (y1 - y0);
        float b = x0 - (m * y0);

        int y_start = std::max(0, static_cast<int>(std::floor(std::min(y0, y1))));
        int y_end = std::min(static_cast<int>(canvas_height) - 1, static_cast<int>(std::floor(std::max(y0, y1))));

        for (int y = y_start; y <= y_end; y++) {
            float x = (m * (y+.5)) + b;
            if (x >= 0 && x <= canvas_width) {
                list_i[y].push_back(x);
            }
        }
    }

    for (auto& intersections : list_i) {
        std::sort(intersections.begin(), intersections.end());
    }

    for (int y = 0; y < canvas_height; y++) {
        const auto& intersections = list_i[y];
        if (intersections.size() < 2) {
            continue;
        }
        for (int i = 0; i < intersections.size(); i += 2) {
            float x0 = intersections[i];
            float x1 = intersections[i + 1];
            if (x0 > x1) {
                std::swap(x0, x1);
            }
            if (paint.peekShader() != nullptr) {
                blitRow2(int(x0), y, int(x1-x0), paint, colors, texs, verts); // spock
                //drawRectRow1({round(x0), float(y), round(x1), float(y+1)}, paint, colors, texs, verts);
            }
            else{
                drawRectRow2({round(x0), float(y), round(x1), float(y+1)}, paint, colors, texs, verts); // drawtri
            }
        }
        
    }
}


void MyCanvas::blitRow2(int x, int y, int count, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[]) {
    std::vector<GPixel> storage(count);
    GShader * s = paint.peekShader();
    if (s != nullptr) {
        s ->shadeRow(x,y,count, storage.data()); // red means it doesn't get into shaderow and white means shaderow is incorrect
    }
    else {// shaderow doesn't work right
        std::fill(storage.begin(), storage.end(), GPixel_PackARGB(200, 200, 0, 0));
    }
    for (int i = 0; i < count; i++) {
        int xi = x+i;
        if (xi >= fDevice.width()) {
            xi = fDevice.width()-1;
        }
        if (xi < 0) {
            xi = 0;
        }
        GPixel* dst = fDevice.getAddr(xi, y);
        if (dst != nullptr) {  // Check if the destination address is valid
            GPixel src = storage[i];
            GPoint point = GPoint({x+i, y});
            *dst = blend2(src, dst, paint, colors, texs, verts, point);
        }
        else {
            GPixel src = storage[i];
            *dst = src;
        }
    }
    
}


GPixel MyCanvas::blend2(GPixel src, GPixel* dst, GPaint paint, const GColor colors[], const GPoint texs[], const GPoint verts[], GPoint point) {
    int dstR = GPixel_GetR(*dst);
    int dstG = GPixel_GetG(*dst);
    int dstB = GPixel_GetB(*dst);
    int dstA = GPixel_GetA(*dst);

    int srcR = GPixel_GetR(src);
    int srcG = GPixel_GetG(src);
    int srcB = GPixel_GetB(src);
    int srcA = GPixel_GetA(src);

    int outR, outG, outB, outA;

    GPixel tex_pixel;
    GPixel color_pixel;

    if (colors != nullptr) {
        float x0 = verts[1].x;
        float y0 = verts[1].y;
        float x1 = verts[2].x;
        float y1 = verts[2].y;
        float x2 = point.x;
        float y2 = point.y;
        float V0area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        x0 = verts[0].x;
        y0 = verts[0].y;
        x1 = verts[2].x;
        y1 = verts[2].y;
        float V1area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        x0 = verts[0].x;
        y0 = verts[0].y;
        x1 = verts[1].x;
        y1 = verts[1].y;
        float V2area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        float V0w = V0area / (V0area + V1area + V2area);
        float V1w = V1area / (V0area + V1area + V2area);
        float V2w = V2area / (V0area + V1area + V2area);
        GColor src = (colors[0] * V0w) + (colors[1] * V1w) + (colors[2] * V2w);
        srcR = (int)(src.r * 255);
        srcG = (int)(src.g * 255);
        srcB = (int)(src.b * 255);
        srcA = (int)(src.a * 255);
        outR = srcR;
        outG = srcB;
        outB = srcB;
        outA = srcA;
        if (outR > outA) {
            outR = outA;
        }
        if (outG > outA) {
            outG = outA;
        }
        if (outB > outA) {
            outB = outA;
        }
        color_pixel = GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
        if (texs == nullptr) {
            return GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
        }

    }
    if (texs != nullptr) {
        float x0 = verts[1].x;
        float y0 = verts[1].y;
        float x1 = verts[2].x;
        float y1 = verts[2].y;
        float x2 = point.x;
        float y2 = point.y;
        float V0area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        x0 = verts[0].x;
        y0 = verts[0].y;
        x1 = verts[2].x;
        y1 = verts[2].y;
        float V1area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        x0 = verts[0].x;
        y0 = verts[0].y;
        x1 = verts[1].x;
        y1 = verts[1].y;
        float V2area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
        float V0w = V0area / (V0area + V1area + V2area);
        float V1w = V1area / (V0area + V1area + V2area);
        float V2w = V2area / (V0area + V1area + V2area);
    // Interpolate texture coordinates using the barycentric coordinates
        float u = (texs[0].x * V0w) + (texs[1].x * V1w) + (texs[2].x * V2w);
        float v = (texs[0].y * V0w) + (texs[1].y * V1w) + (texs[2].y * V2w);

        if (u >= fDevice.width()) {
            u = fDevice.width() -1;
        }
        if (u < 0) {
            u = 0;
        }
        if (v >= fDevice.height()) {
            v = fDevice.height() -1;
        }
        if (v < 0) {
            v = 0;
        }

        GPixel texColorA = GPixel_GetA(*fDevice.getAddr(u,v));

        outR = (int)(srcR * (1 - texColorA / 255.0f) + GPixel_GetR(*fDevice.getAddr(u,v)) * (texColorA / 255.0f));
        outG = (int)(srcG * (1 - texColorA / 255.0f) + GPixel_GetG(*fDevice.getAddr(u,v)) * (texColorA / 255.0f));
        outB = (int)(srcB * (1 - texColorA / 255.0f) + GPixel_GetB(*fDevice.getAddr(u,v)) * (texColorA / 255.0f));
        outA = (int)(srcA * (1 - texColorA / 255.0f) + GPixel_GetA(*fDevice.getAddr(u,v)) * (texColorA / 255.0f));
        if (outR > outA) {
            outR = outA;
        }
        if (outG > outA) {
            outG = outA;
        }
        if (outB > outA) {
            outB = outA;
        }
        tex_pixel = GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
        if (colors == nullptr && paint.peekShader() != nullptr) {
            
            return GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
        }
    }


    
    switch (paint.getBlendMode()) {
        case GBlendMode::kClear:
            return GPixel_PackARGB(0, 0, 0, 0);

        case GBlendMode::kSrc:
            return src;

        case GBlendMode::kDst:
            return *dst;

        case GBlendMode::kSrcOver: // S + (1 - Sa) * D
            outR = (srcR * srcA + dstR * (255 - srcA)) / 255;
            outG = (srcG * srcA + dstG * (255 - srcA)) / 255;
            outB = (srcB * srcA + dstB * (255 - srcA)) / 255;
            outA = clamp(dstA + srcA, 0, 255);
            break;

        case GBlendMode::kDstOver: // D + (1 - Da) * S
            outR = (dstR * dstA + srcR * (255 - dstA)) / 255;
            outG = (dstG * dstA + srcG * (255 - dstA)) / 255;
            outB = (dstB * dstA + srcB * (255 - dstA)) / 255;
            outA = clamp(dstA + srcA, 0, 255);
            break;

        case GBlendMode::kSrcIn: // Da * S
            outR = (srcR * dstA) / 255;
            outG = (srcG * dstA) / 255;
            outB = (srcB * dstA) / 255;
            outA = (srcA * dstA) / 255;
            break;

        case GBlendMode::kDstIn: // Sa * D
            outR = (dstR * srcA) / 255;
            outG = (dstG * srcA) / 255;
            outB = (dstB * srcA) / 255;
            outA = (dstA * srcA) / 255;
            break;

        case GBlendMode::kSrcOut: // (1 - Da) * S
            outR = (srcR * (255 - dstA)) / 255;
            outG = (srcG * (255 - dstA)) / 255;
            outB = (srcB * (255 - dstA)) / 255;
            outA = (srcA * (255 - dstA)) / 255;
            break;

        case GBlendMode::kDstOut: // (1 - Sa) * D
            outR = (dstR * (255 - srcA)) / 255;
            outG = (dstG * (255 - srcA)) / 255;
            outB = (dstB * (255 - srcA)) / 255;
            outA = (dstA * (255 - srcA)) / 255;
            break;

        case GBlendMode::kSrcATop: // Da * S + (1 - Sa) * D
            outR = (dstA * srcR + (255 - srcA) * dstR) / 255;
            outG = (dstA * srcG + (255 - srcA) * dstG) / 255;
            outB = (dstA * srcB + (255 - srcA) * dstB) / 255;
            outA = clamp(dstA + (srcA * (255 - dstA) / 255), 0, 255);
            break;

        case GBlendMode::kDstATop: // Sa * D + (1 - Da) * S
            outR = (srcA * dstR + (255 - dstA) * srcR) / 255;
            outG = (srcA * dstG + (255 - dstA) * srcG) / 255;
            outB = (srcA * dstB + (255 - dstA) * srcB) / 255;
            outA = clamp(srcA + (dstA * (255 - srcA) / 255), 0, 255);
            break;

        case GBlendMode::kXor: // (1 - Sa) * D + (1 - Da) * S
            outR = ((255 - srcA) * dstR + (255 - dstA) * srcR) / 255;
            outG = ((255 - srcA) * dstG + (255 - dstA) * srcG) / 255;
            outB = ((255 - srcA) * dstB + (255 - dstA) * srcB) / 255;
            outA = clamp((255 - srcA) * dstA + (255 - dstA) * srcA, 0, 255);
            break;
    }
    if (outR > outA) {
        outR = outA;
    }
    if (outG > outA) {
        outG = outA;
    }
    if (outB > outA) {
        outB = outA;
    }

    //return GPixel_PackARGB(0,200,0,220);
    
    return GPixel_PackARGB(clamp(outA, 0, 255), clamp(outR, 0, 255), clamp(outG, 0, 255), clamp(outB, 0, 255));
}



void MyCanvas::drawRectRow2(const GRect& rect, const GPaint& paint, const GColor colors[], const GPoint texs[], const GPoint verts[]) {
    GRect transformedRect;
    GPixel* addr = nullptr;


    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    currentMatrix.mapPoints((GPoint*)&transformedRect, (const GPoint*)&rect, 2);

    transformedRect = rect;

    int i0 = (0 > transformedRect.left) ? 0 : (int)round(transformedRect.left);
    int i1 = (transformedRect.right > fDevice.width()) ? fDevice.width() : (int)round(transformedRect.right);
    int ii0 = (0 > transformedRect.top) ? 0 : (int)round(transformedRect.top);
    int ii1 = (transformedRect.bottom > fDevice.height()) ? fDevice.height() : (int)round(transformedRect.bottom);

    const GPoint points[] = {
    {i0, ii0},    // Top-left
    {i1, ii0},   // Top-right
    {i1, ii1}, // Bottom-right
    {i0, ii1}   // Bottom-left
    };


    int srcR;
    int srcG;
    int srcB;
    int srcA;

    GPoint v0 = GPoint({490, 10});
    GPoint v2 = GPoint({100, 250});
    GPoint v1 = GPoint({400, 400});


    unsigned int outR, outG, outB, outA;


    for (int ii = ii0; ii < ii1; ++ii) {
        for (int i = i0; i < i1; ++i) {
            //printf("Vert[0]: %f, %f\n", verts[0].x, verts[0].y);
            //printf("Vert[1]: %f, %f\n", verts[1].x, verts[1].y);
            //printf("Vert[2]: %f, %f\n", verts[2].x, verts[2].y);
            
            float x0 = v1.x;
            float y0 = v1.y;
            float x1 = v2.x;
            float y1 = v2.y;
            float x2 = i;
            float y2 = ii;
            float V0area = (0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1))) + .2;
            x0 = v0.x;
            y0 = v0.y;
            x1 = v2.x;
            y1 = v2.y;
            float V1area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
            x0 = v0.x;
            y0 = v0.y;
            x1 = v1.x;
            y1 = v1.y;
            float V2area = 0.5f * std::abs(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1));
            float V0w = (V0area / (V0area + V1area + V2area));
            //printf("%f\n", V0w);
            float V1w = V1area / (V0area + V1area + V2area);
            //printf("%f\n", V1w);
            float V2w = V2area / (V0area + V1area + V2area);
            //printf("%f\n", V2w);
            GColor src = (colors[0] * V0w) + (colors[1] * V1w) + (colors[2] * V2w);
            srcR = (int)(src.r * 255);
            srcG = (int)(src.g * 255);
            srcB = (int)(src.b * 255);
            srcA = (int)(src.a * 255);

            if (i >= fDevice.width()) {
                i = fDevice.width() - 1;
            }
            if (i < 0) {
                i = 0;
            }
            addr = fDevice.getAddr(i, ii);

            int dstR = GPixel_GetR(*addr);
            int dstG = GPixel_GetG(*addr);
            int dstB = GPixel_GetB(*addr);
            int dstA = GPixel_GetA(*addr);

            outR = srcR;
            outG = srcG;
            outB = srcB;
            outA = srcA;

           if (outA > 255) {
            outA = 255;
           }
            
            
            *addr = GPixel_PackARGB(outA, std::min(outR, outA), std::min(outG, outA), std::min(outB,outA));
        }
    }
}




void MyCanvas::drawConvexPolygonStroke(const GPoint points[], int count, const GPaint& paint) {
    GColor color = paint.getColor();
    GBlendMode blend_mode = paint.getBlendMode();
    GShader * shader = paint.peekShader();
    if (paint.peekShader() != nullptr) {
        const GMatrix& ctm = currentMatrix;
        shader->setContext(ctm);
    }

    GPoint points1[count];

    
    for (int i = 0; i < count; ++i) {
        points1[i] = points[i];
    }
    
    currentMatrix.mapPoints(points1, points, count);

    points = points1;

    if (count < 3) {
        return;
    }

    float canvas_width = fDevice.width();
    float canvas_height = fDevice.height();

    std::vector<Edge> edges;

    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;

        // if i is inside and j is outside (left) 
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, 0, y});

                        edges.push_back({0, y, 0, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (right)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = m * x + b;
                        edges.push_back({points[i].x, points[i].y, canvas_width, y});

                        edges.push_back({canvas_width, y, canvas_width, points[j].y});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (top)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, 0});

                        //edges.push_back({x, 0, points[j].x, 0});
                        continue;
                    }
                }
            }
        }

        // if i is inside and j is outside (bottom)
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({points[i].x, points[i].y, x, canvas_height});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (bottom) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = canvas_height;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (top) and j is inside
        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y = 0;
                        float x = (y - b) / m;
                        
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is outside (left) and j is inside
        if (points[i].x < 0) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = 0;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }

        // if i is outside (right) and j is inside
        if (points[i].x > canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y <= canvas_height && points[j].y >= 0) {
                        //float m = (points[j].y - points[i].y) / (points[j].x - points[i].x);
                        //float b = points[i].y - m * points[i].x;
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float x = canvas_width;
                        float y = (m * x) + b;
                        
                        edges.push_back({x, points[i].y, x, y});
                        edges.push_back({x, y, points[j].x, points[j].y});

                        //edges.push_back({x, canvas_height, points[j].x, canvas_height});
                        continue;
                    }
                }
            }
        }


        // if i is top and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 > canvas_width) {
                            continue;
                        }
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        // if i is top and j is bottom
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = 0;
                        float y2 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        continue;
                    }
                //}
            }
        //}



        // if i is top and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y < 0) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        // y = 0 and x = (y - b) / m
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (x1 < 0) {
                            continue;
                        }
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, x2, points[j].y});

                        continue;
                    //}
                }
            }
        //}


        
        // if i is right and j is right
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({canvas_width, points[i].y, canvas_width, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        



        
        // if i is right and j is bottom
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }
                        edges.push_back({canvas_width, points[i].y, x2, y2});
                        edges.push_back({x2, y2, x1, y1});

                        continue;
                    }
                //}
            //}
        }



        // if i is right and j is left
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = canvas_width;
                        float y1 = (m * x1) + b;

                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({canvas_width, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});


                        continue;
                        
                    //}
                }
            //}
        }



        // if i is right and j is top
        if (points[i].x > canvas_width) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x2 = 0;
                        float y2 = (m * x2) + b;
                        float y1 = 0;
                        float x1 = (y1 - b) / m;
                        if (y2 > canvas_height) {
                            continue;
                        }


                        edges.push_back({0, points[i].y, 0, y2});
                        edges.push_back({0, y2, x1, 0});
                        continue;
                        
                    }
                //}
            //}
        }



        // if i is bottom and j is right
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});


                        
                        
                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is left
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float y1 = canvas_height;
                        float x1 = (y1 - b) / m;
                        float x2 = 0;
                        float y2 = (m * x2) + b;

                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, 0, points[j].y});




                        continue;
                    //}
                }
            }
        //}



        // if i is bottom and j is top
        //if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y > canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        float y1 = canvas_height;
                        float y2 = 0;
                        float x1 = (y1 - b) / m;
                        float x2 = (y2 - b) / m;
                        edges.push_back({x1, y1, x2, y2});


                        
                        continue;
                    }
                //}
            }
        //}



        // if i is left and j is right
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x > canvas_width) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float x2 = canvas_width;
                        float y2 = (m * x2) + b;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        edges.push_back({x2, y2, canvas_width, points[j].y});



                        continue;
                    //}
                }
            //}
        }



        // if i is left and j is bottom
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y > canvas_height) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = canvas_height;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        // if i is left and j is left
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x < 0) {
                    //if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({0, points[i].y, 0, points[j].y});
                        continue;
                    //}
                }
            //}
        }
        // if i is left and j is top
        if (points[i].x < 0) {
            //if (points[i].y >= 0 && points[i].y <= canvas_height) {
                //if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y < 0) {
                        float m = (points[j].x - points[i].x) / (points[j].y - points[i].y);
                        float b = points[i].x - (m * points[i].y);
                        
                        float x1 = 0;
                        float y1 = (m * x1) + b;

                        float y2 = 0;
                        float x2 = (y1 - b) / m;

                        edges.push_back({0, points[i].y, x1, y1});
                        edges.push_back({x1, y1, x2, y2});
                        continue;
                    }
                //}
            //}
        }



        if (points[i].x >= 0 && points[i].x <= canvas_width) {
            if (points[i].y >= 0 && points[i].y <= canvas_height) {
                if (points[j].x >= 0 && points[j].x <= canvas_width) {
                    if (points[j].y >= 0 && points[j].y <= canvas_height) {
                        edges.push_back({points[i].x, points[i].y, points[j].x, points[j].y});
                    }
                }
            }
        }
    }
    

    std::vector<std::vector<float>> list_i(static_cast<size_t>(canvas_height));

    for (const auto& edge : edges) {
        float x0 = edge.x1;
        float y0 = edge.y1;
        float x1 = edge.x2;
        float y1 = edge.y2;

        if (y0 == y1) {
            continue;
        }

        float m = (x1 - x0) / (y1 - y0);
        float b = x0 - (m * y0);

        int y_start = std::max(0, static_cast<int>(std::floor(std::min(y0, y1))));
        int y_end = std::min(static_cast<int>(canvas_height) - 1, static_cast<int>(std::floor(std::max(y0, y1))));

        for (int y = y_start; y <= y_end; y++) {
            float x = (m * (y+.5)) + b;
            if (x >= 0 && x <= canvas_width) {
                list_i[y].push_back(x);
            }
        }
    }

    for (auto& intersections : list_i) {
        std::sort(intersections.begin(), intersections.end());
    }

    for (int y = 0; y < canvas_height; y++) {
        const auto& intersections = list_i[y];
        if (intersections.size() < 2) {
            continue;
        }
     
        for (int i = 0; i < intersections.size(); i += 2) {
            float x0 = intersections[i];
            float x1 = intersections[i + 1];
            if (x0 > x1) {
                std::swap(x0, x1);
            }
            if (paint.peekShader() != nullptr) {
                //blitRow(int(x0), y, int(x1-x0), paint); // spock
                if (x0+5 >= canvas_width) {
                    blitRow(int(x0), y, int(x0), paint); // spock
                }
                else{
                blitRow(int(x0), y, int(5), paint); // spock
                }
            }
            else{
                //drawRectRow({round(x0), float(y), round(x1), float(y+1)}, paint); // color clock
                drawRectRow({round(x0), float(y), round(x0+5), float(y+1)}, paint); // color clock
            }
        }
        
    }
}

/*
void MyFinal::createSweepGradient(GPoint center, float startRadians,
                                                         const GColor colors[], int count) {
                                                            return new GFinal();
                                                         }
                                                         */
/// return GFinal
/// ./image -e expected/ -v