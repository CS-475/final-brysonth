#include "myShader.h"
#include "include/GBitmap.h" 
#include "include/GMatrix.h"
#include "include/GPixel.h"
#include "include/GShader.h"
//#include "myCanvas.cpp"
//#include "myCanvas.h"
//#include <vector>
//#include <iostream>
//#include <algorithm>
//#include <cassert>
//#include <optional>


myShader::myShader(const GBitmap& bitmap, const GMatrix& localMatrix) {
    fBitmap = bitmap;
    fLocalMatrix = localMatrix;
    gradient = false;
}

myShader::myShader(const GBitmap& bitmap, const GMatrix& localMatrix, GTileMode tilemode1) {
    fBitmap = bitmap;
    fLocalMatrix = localMatrix;
    gradient = false;
    tilemode = tilemode1;
}


myShader::myShader(GPoint p0, GPoint p1, std::vector<GColor> colors, int count) {
    p0_f = p0;
    p1_f = p1;
    color_list_f = colors;
    count_f = count;
    gradient = true;
}

myShader::myShader(GPoint p0, GPoint p1, std::vector<GColor> colors, int count, GTileMode tilemode1) {
    p0_f = p0;
    p1_f = p1;
    color_list_f = colors;
    count_f = count;
    gradient = true;
    tilemode = tilemode1;
    rel_pos = false;
}

myShader::myShader(GPoint p0, GPoint p1, GColor color1, GColor color2) {
    p0_f = p0;
    p1_f = p1;
    std::vector<GColor> list = {color1, color2};
    color_list_f = list;
    count_f = 2;
    gradient = true;
    rel_pos = false;
}
/*
myShader::myShader(std::vector<GPoint> rpos, std::vector<GColor> colors, int count) {
    //p0_f = rpos[0];
    //p1_f = rpos[count - 1];
    point_list_f = rpos;
    color_list_f = colors;
    count_f = count;
    gradient = true;
    rel_pos = true;
}
*/

myShader::myShader(const GBitmap& bitmap, const GMatrix& localMatrix, bool avg) {
    fBitmap = bitmap;
    fLocalMatrix = localMatrix;
    gradient = false;
    bit_avg = true;
}




void myShader::setBitmap(const GBitmap& bitmap) {
    fBitmap = bitmap;
}

void myShader::setLocalMatrix(const GMatrix& localMatrix) {
    fLocalMatrix = localMatrix;
}

const GBitmap& myShader::getBitmap() {
    return fBitmap;
}

const GMatrix& myShader::getLocalMatrix() {
    return fLocalMatrix;
}

// Return true iff all of the GPixels that may be returned by this shader will be opaque.
bool myShader::isOpaque() {
    unsigned int* pixels = fBitmap.pixels();
    int width = fBitmap.width();
    int height = fBitmap.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            GPixel pixel = pixels[y * width + x];
            if (GPixel_GetA(pixels[y * width + x]) < 255) {
                return false;
            }
        }
    }
    return true;
}

// The draw calls in GCanvas must call this with the CTM before any calls to shadeSpan().
bool myShader::setContext(const GMatrix& ctm) {
    // verify output is inv is inverse of input
    fCTM = GMatrix::Concat(ctm, fLocalMatrix);
    //fCTM = GMatrix::Concat(fLocalMatrix, ctm);
    //fCTM = ctm;
    inv = fCTM.invert();
    if (!inv.has_value()) {
        return false;
    }
    return true;
}


/**
 *  Given a row of pixels in device space [x, y] ... [x + count - 1, y], return the
 *  corresponding src pixels in row[0...count - 1]. The caller must ensure that row[]
 *  can hold at least [count] entries.
 */


void myShader::shadeRow(int x, int y, int count, GPixel row[]) {
    //printf("a \n");
    auto clamp = [](int value, int minVal, int maxVal) {
        return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
    };
    //printf("b \n");

    if (!gradient) {
        if (bit_avg == true) {
            int bitmapWidth = fBitmap.width();
            int bitmapHeight = fBitmap.height();
            unsigned int* pixels = fBitmap.pixels();

            if (pixels == nullptr || bitmapWidth <= 0 || bitmapHeight <= 0) {
                for (int i = 0; i < count; ++i) {
                    row[i] = GPixel();
                }
                return;
            }

            GMatrix inverseMatrix;
            inverseMatrix = inv.value();

            for (int i = 0; i < count; ++i) {
                float srcX = (inverseMatrix[0] * (x + i + 0.5f)) + (inverseMatrix[2] * (y + 0.5f)) + inverseMatrix[4];
                float srcY = (inverseMatrix[1] * (x + i + 0.5f)) + (inverseMatrix[3] * (y + 0.5f)) + inverseMatrix[5];

                int clampedX, clampedY;

                if (tilemode == GTileMode::kRepeat) {
                    clampedX = (static_cast<int>(srcX) % bitmapWidth + bitmapWidth) % bitmapWidth;
                    clampedY = (static_cast<int>(srcY) % bitmapHeight + bitmapHeight) % bitmapHeight;
                } else if (tilemode == GTileMode::kMirror) {
                    clampedX = (srcX < 0) ? -static_cast<int>(srcX) - 1 : static_cast<int>(srcX);
                    clampedY = (srcY < 0) ? -static_cast<int>(srcY) - 1 : static_cast<int>(srcY);

                    if (clampedX / bitmapWidth % 2 == 1) {
                        clampedX = bitmapWidth - 1 - (clampedX % bitmapWidth);
                    } else {
                        clampedX = clampedX % bitmapWidth;
                    }

                    if (clampedY / bitmapHeight % 2 == 1) {
                        clampedY = bitmapHeight - 1 - (clampedY % bitmapHeight);
                    } else {
                        clampedY = clampedY % bitmapHeight;
                    }
                } else {
                    clampedX = clamp(static_cast<int>(srcX), 0, bitmapWidth - 1);
                    clampedY = clamp(static_cast<int>(srcY), 0, bitmapHeight - 1);
                }

                int leftX = clamp(clampedX - 1, 0, bitmapWidth - 1);
                int rightX = clamp(clampedX + 1, 0, bitmapWidth - 1);
                int topY = clamp(clampedY - 1, 0, bitmapHeight - 1);
                int bottomY = clamp(clampedY + 1, 0, bitmapHeight - 1);

                int leftIndex = topY * bitmapWidth + leftX;
                int rightIndex = topY * bitmapWidth + rightX;
                int topIndex = clampedY * bitmapWidth + leftX;
                int bottomIndex = clampedY * bitmapWidth + rightX;

                unsigned int leftColor = pixels[leftIndex];
                unsigned int rightColor = pixels[rightIndex];
                unsigned int topColor = pixels[topIndex];
                unsigned int bottomColor = pixels[bottomIndex];

                float weightLeft = 1.0f - std::abs(srcX - leftX);
                float weightRight = 1.0f - std::abs(srcX - rightX);
                float weightTop = 1.0f - std::abs(srcY - topY);
                float weightBottom = 1.0f - std::abs(srcY - bottomY);

                float totalWeight = weightLeft + weightRight + weightTop + weightBottom;
                weightLeft /= totalWeight;
                weightRight /= totalWeight;
                weightTop /= totalWeight;
                weightBottom /= totalWeight;

                unsigned char r = 0, g = 0, b = 0, a = 0;

                auto extractColor = [](unsigned int pixel) {
                    return GColor{
                        (float)((pixel >> 16) & 0xFF) / 255.0f,
                        (float)((pixel >> 8) & 0xFF) / 255.0f,
                        (float)(pixel & 0xFF) / 255.0f,
                        (float)((pixel >> 24) & 0xFF) / 255.0f
                    };
                };

                GColor leftColorF = extractColor(leftColor);
                GColor rightColorF = extractColor(rightColor);
                GColor topColorF = extractColor(topColor);
                GColor bottomColorF = extractColor(bottomColor);

                r = (leftColorF.r * weightLeft + rightColorF.r * weightRight + topColorF.r * weightTop + bottomColorF.r * weightBottom) * 255.0f;
                g = (leftColorF.g * weightLeft + rightColorF.g * weightRight + topColorF.g * weightTop + bottomColorF.g * weightBottom) * 255.0f;
                b = (leftColorF.b * weightLeft + rightColorF.b * weightRight + topColorF.b * weightTop + bottomColorF.b * weightBottom) * 255.0f;
                a = (leftColorF.a * weightLeft + rightColorF.a * weightRight + topColorF.a * weightTop + bottomColorF.a * weightBottom) * 255.0f;

                r = std::min(r, a);
                g = std::min(g, a);
                b = std::min(b, a);

                row[i] = GPixel_PackARGB(a, r, g, b);
            }
            return;
        }
        
        int bitmapWidth = fBitmap.width();
        int bitmapHeight = fBitmap.height();
        unsigned int* pixels = fBitmap.pixels();
        //printf("one1 \n");

        if (pixels == nullptr || bitmapWidth <= 0 || bitmapHeight <= 0) {
            //printf("try1 \n");
            for (int i = 0; i < count; ++i) {
                //printf("one2 \n");
                row[i] = GPixel(); 
            }
            return;
        }
        //printf("try2 \n");
        GMatrix inverseMatrix;
        //if (inv.has_value()) {
            inverseMatrix = inv.value();
        //}
        //else {
            //return;
        //}
        //printf("try3 \n");
        for (int i = 0; i < count; ++i) {
            //printf("try4 \n");
            float srcX = (inverseMatrix[0] * (x + i + 0.5f)) + (inverseMatrix[2] * (y + 0.5f)) + inverseMatrix[4];
            float srcY = (inverseMatrix[1] * (x + i + 0.5f)) + (inverseMatrix[3] * (y + 0.5f)) + inverseMatrix[5];
            //printf("one3 \n");

            int clampedX, clampedY;

            if (tilemode == GTileMode::kRepeat) {
                clampedX = (static_cast<int>(srcX) % bitmapWidth + bitmapWidth) % bitmapWidth;
                clampedY = (static_cast<int>(srcY) % bitmapHeight + bitmapHeight) % bitmapHeight;
                //printf("one4 \n");
            } else if (tilemode == GTileMode::kMirror) {
                clampedX = (srcX < 0) ? -static_cast<int>(srcX) - 1 : static_cast<int>(srcX);
                clampedY = (srcY < 0) ? -static_cast<int>(srcY) - 1 : static_cast<int>(srcY);
                //printf("one5 \n");

                if (clampedX / bitmapWidth % 2 == 1) {
                    clampedX = bitmapWidth - 1 - (clampedX % bitmapWidth);
                    //printf("one6 \n");
                } else {
                    clampedX = clampedX % bitmapWidth;
                    //printf("one7 \n");
                }

                if (clampedY / bitmapHeight % 2 == 1) {
                    clampedY = bitmapHeight - 1 - (clampedY % bitmapHeight);
                    //printf("one8 \n");
                } else {
                    clampedY = clampedY % bitmapHeight;
                    //printf("one9 \n");
                }
            } else {
                clampedX = clamp(static_cast<int>(srcX), 0, bitmapWidth - 1);
                clampedY = clamp(static_cast<int>(srcY), 0, bitmapHeight - 1);
                //printf("one10 \n");
            }

            int index = clampedY * bitmapWidth + clampedX;
            //printf("one11 \n");
            if (index < 0) {
                index = 0;
            }
            if (index < 0 || index >= (bitmapWidth * bitmapHeight)) {
                row[i] = GPixel();
                continue;
            }
            //printf("one12 \n");

            row[i] = pixels[index];
            //printf("one13 \n");
        }
        
    } 

else {
    if (tilemode == GTileMode::kMirror) {
        GPoint points[] = { {p0_f.x + 0.5f, p0_f.y + 0.5f}, {p1_f.x + 0.5f, p1_f.y + 0.5f} };
        float dx = p1_f.x - p0_f.x;
        float dy = p1_f.y - p0_f.y;
        //printf("two1 \n");

        std::vector<GColor> colors = color_list_f;
        int count1 = colors.size();
        for (int i = 1; i < count1; i++) {
            colors.push_back(colors[count1 - 1 - i]);
        }
        int colorCount = colors.size();

        for (int i = 0; i < count; i++) {
            GMatrix bigMatrix(dx, -dy, p0_f.x, dy, dx, p0_f.y);
            auto inverseResult = bigMatrix.invert();

            if (!inverseResult.has_value()) {
                continue;
            }

            GMatrix inverseMatrix = inverseResult.value();
            
            float srcX = (inverseMatrix[0] * (x + i + 0.5f)) + (inverseMatrix[2] * (y + 0.5f)) + inverseMatrix[4];
            float srcY = (inverseMatrix[1] * (x + i + 0.5f)) + (inverseMatrix[3] * (y + 0.5f)) + inverseMatrix[5];

            float t = static_cast<float>(i) / (count - 1);
            GColor color;

            if (colorCount > 1) {
                int colorIndex = std::min(static_cast<int>(t * (colorCount - 1)), colorCount - 2);
                float localT = (t * (colorCount - 1)) - colorIndex;

                color.r = colors[colorIndex].r * (1 - localT) + colors[colorIndex + 1].r * localT;
                color.g = colors[colorIndex].g * (1 - localT) + colors[colorIndex + 1].g * localT;
                color.b = colors[colorIndex].b * (1 - localT) + colors[colorIndex + 1].b * localT;
                color.a = colors[colorIndex].a * (1 - localT) + colors[colorIndex + 1].a * localT;
            } else {
                color = colors[0]; 
            }

            color.a = std::max(color.a, std::max({color.r, color.g, color.b}));

            row[i] = GPixel_PackARGB(color.a * 255, color.r * 255, color.g * 255, color.b * 255);
        }
}

    else if (tilemode == GTileMode::kRepeat) {
        GPoint points[] = { {p0_f.x + 0.5f, p0_f.y + 0.5f}, {p1_f.x + 0.5f, p1_f.y + 0.5f} };
        float dx = p1_f.x - p0_f.x;
        float dy = p1_f.y - p0_f.y;
        //printf("three \n");

        std::vector<GColor> colors = color_list_f;
        int count1 = colors.size();
        for (int i = 1; i < count1; i++) {
            colors.push_back(colors[i]);
        }
        int colorCount = colors.size();

        for (int i = 0; i < count; i++) {
            GMatrix bigMatrix(dx, -dy, p0_f.x, dy, dx, p0_f.y);
            auto inverseResult = bigMatrix.invert();

            if (!inverseResult.has_value()) {
                continue;
            }

            GMatrix inverseMatrix = inverseResult.value();
            
            float srcX = (inverseMatrix[0] * (x + i + 0.5f)) + (inverseMatrix[2] * (y + 0.5f)) + inverseMatrix[4];
            float srcY = (inverseMatrix[1] * (x + i + 0.5f)) + (inverseMatrix[3] * (y + 0.5f)) + inverseMatrix[5];

            float t = static_cast<float>(i) / (count - 1);
            GColor color;

            if (colorCount > 1) {
                int colorIndex = std::min(static_cast<int>(t * (colorCount - 1)), colorCount - 2);
                float localT = (t * (colorCount - 1)) - colorIndex;

                color.r = colors[colorIndex].r * (1 - localT) + colors[colorIndex + 1].r * localT;
                color.g = colors[colorIndex].g * (1 - localT) + colors[colorIndex + 1].g * localT;
                color.b = colors[colorIndex].b * (1 - localT) + colors[colorIndex + 1].b * localT;
                color.a = colors[colorIndex].a * (1 - localT) + colors[colorIndex + 1].a * localT;
            } else {
                color = colors[0]; 
            }

            color.a = std::max(color.a, std::max({color.r, color.g, color.b}));

            row[i] = GPixel_PackARGB(color.a * 255, color.r * 255, color.g * 255, color.b * 255);
        }
    }
    
    else {
        if (rel_pos == true) {
            point_list_f = {{1,1}, {100, 100}, {200,200}};


            float dx = p1_f.x - p0_f.x;
            float dy = p1_f.y - p0_f.y;

            float length = std::sqrt(dx * dx + dy * dy);
            
            std::vector<GColor> colors = color_list_f;
            std::vector<GPoint> points = point_list_f;

            int colorCount = colors.size();

            float gradientLength = std::sqrt(dx * dx + dy * dy);
            float unitDx = dx / gradientLength;
            float unitDy = dy / gradientLength;

            if (!inv.has_value()) {
                return;
            }

            for (int i = 0; i < count; i++) {
                float srcX = (inv.value()[0] * (x + i + 0.5f)) + (inv.value()[2] * (y + 0.5f)) + inv.value()[4];
                float srcY = (inv.value()[1] * (x + i + 0.5f)) + (inv.value()[3] * (y + 0.5f)) + inv.value()[5];

                float t = ((srcX - p0_f.x) * unitDx + (srcY - p0_f.y) * unitDy) / gradientLength;
                t = std::clamp(t, 0.0f, 1.0f); 

                float intersectionX = p0_f.x + t * dx;
                float intersectionY = p0_f.y + t * dy;

                float distToStart = std::sqrt((intersectionX - p0_f.x) * (intersectionX - p0_f.x) + 
                                            (intersectionY - p0_f.y) * (intersectionY - p0_f.y));

                float normalizedT = distToStart / length;
                normalizedT = std::clamp(normalizedT, 0.0f, 1.0f); 

                GColor color;
                if (colorCount > 1) {
                    int colorIndex = 0;

                    for (int j = 0; j < points.size() - 1; j++) {
                        float distStartToPos = std::sqrt((points[j].x - p0_f.x) * (points[j].x - p0_f.x) +
                                                        (points[j].y - p0_f.y) * (points[j].y - p0_f.y)) / length;
                        float distEndToPos = std::sqrt((points[j + 1].x - p0_f.x) * (points[j + 1].x - p0_f.x) +
                                                    (points[j + 1].y - p0_f.y) * (points[j + 1].y - p0_f.y)) / length;

                        if (normalizedT >= distStartToPos && normalizedT < distEndToPos) {
                            colorIndex = j;
                            break;
                        }
                    }

                    float localT = (normalizedT - (points[colorIndex].x - p0_f.x) / length) / 
                                ((points[colorIndex + 1].x - points[colorIndex].x) / length);

                    color.r = colors[colorIndex].r * (1 - localT) + colors[colorIndex + 1].r * localT;
                    color.g = colors[colorIndex].g * (1 - localT) + colors[colorIndex + 1].g * localT;
                    color.b = colors[colorIndex].b * (1 - localT) + colors[colorIndex + 1].b * localT;
                    color.a = colors[colorIndex].a * (1 - localT) + colors[colorIndex + 1].a * localT;
                } else {
                    color = colors[0];
                }

                color.a = std::max(color.a, std::max(color.r, std::max(color.g, color.b)));

                int a = color.a * 255;
                if (a > 255) {
                    a = 255;
                }
                if (a < color.r * 255) {
                    color.r = color.a;
                }
                if (a < color.g * 255) {
                    color.g = color.a;
                }
                if (a < color.b * 255) {
                    color.b = color.a;
                }


                row[i] = GPixel_PackARGB(a, color.r * 255, color.g * 255, color.b * 255);
                //return;
            }
        }
        
        else {
            float dx = p1_f.x - p0_f.x;
            float dy = p1_f.y - p0_f.y;

            float length = std::sqrt(dx * dx + dy * dy);
            
            std::vector<GColor> colors = color_list_f;
            int colorCount = colors.size();

            float gradientLength = std::sqrt(dx * dx + dy * dy);
            float unitDx = dx / gradientLength;
            float unitDy = dy / gradientLength;

            if (!inv.has_value()) {
                return;
            }

            for (int i = 0; i < count; i++) {
                float srcX = (inv.value()[0] * (x + i + 0.5f)) + (inv.value()[2] * (y + 0.5f)) + inv.value()[4];
                float srcY = (inv.value()[1] * (x + i + 0.5f)) + (inv.value()[3] * (y + 0.5f)) + inv.value()[5];

                float t = ((srcX - p0_f.x) * unitDx + (srcY - p0_f.y) * unitDy) / gradientLength;
                t = std::clamp(t, 0.0f, 1.0f); 

                float intersectionX = p0_f.x + t * dx;
                float intersectionY = p0_f.y + t * dy;


                float distToStart = std::sqrt((intersectionX - p0_f.x) * (intersectionX - p0_f.x) + 
                                            (intersectionY - p0_f.y) * (intersectionY - p0_f.y));

                float normalizedT = distToStart / length;
                normalizedT = std::clamp(normalizedT, 0.0f, 1.0f); 

                GColor color;
                if (colorCount > 1) {
                    int colorIndex = std::min(static_cast<int>(normalizedT * (colorCount - 1)), colorCount - 2);
                    float localT = (normalizedT * (colorCount - 1)) - colorIndex;

                    color.r = colors[colorIndex].r * (1 - localT) + colors[colorIndex + 1].r * localT;
                    color.g = colors[colorIndex].g * (1 - localT) + colors[colorIndex + 1].g * localT;
                    color.b = colors[colorIndex].b * (1 - localT) + colors[colorIndex + 1].b * localT;
                    color.a = colors[colorIndex].a * (1 - localT) + colors[colorIndex + 1].a * localT;
                } else {
                    color = colors[0]; 
                }

                color.a = std::max(color.a, std::max(color.r, std::max(color.g, color.b)));

                row[i] = GPixel_PackARGB(color.a * 255, color.r * 255, color.g * 255, color.b * 255);
            }
        }
        
    }

}
}








std::shared_ptr<GShader> GCreateBitmapShader(const GBitmap& bitmap, const GMatrix& localm) {
    
    assert(bitmap.width() > 0);
    assert(bitmap.height() > 0);
    GMatrix m = localm; //* GMatrix::Scale(bitmap.width(), bitmap.height());

    GBitmap bm = bitmap;
    //bm.readFromFile("apps/spock.png");
    
    
    return std::make_shared<myShader>(bm, m);
    //return std::make_shared<myShader>(bm, m);
    
}
std::shared_ptr<GShader> GCreateLinearGradient(GPoint p0, GPoint p1, const GColor colors[], int count) {
    std::vector<GColor> colorVector(colors, colors + count);
    return std::make_shared<myShader>(p0, p1, colorVector, count);
}

std::shared_ptr<GShader> GCreateLinearGradient(GPoint p0, GPoint p1, const GColor colors[] , int count, GTileMode tilemode) {
    std::vector<GColor> colorVector(colors, colors + count);
    return std::make_shared<myShader>(p0, p1, colorVector, count, tilemode);
}

std::shared_ptr<GShader> GCreateLinearGradient(GPoint p0, GPoint p1, const GColor color1, const GColor color2) {
    return std::make_shared<myShader>(p0, p1, color1, color2);
}

std::shared_ptr<GShader> GCreateBitmapShader(const GBitmap& bitmap, const GMatrix& localm, GTileMode tilemode) {
    assert(bitmap.width() > 0);
    assert(bitmap.height() > 0);
    GMatrix m = localm; //* GMatrix::Scale(bitmap.width(), bitmap.height());

    GBitmap bm = bitmap;
    //bm.readFromFile("apps/spock.png");
    
    
    return std::make_shared<myShader>(bm, m, tilemode);
}



/*
std::shared_ptr<GShader> GCreateLinearGradient(std::vector<GPoint> rpos, const GColor colors, int count) {
    //std::vector<GColor> colorVector(colors, colors + count);
    //std::vector<GPoint> pointVector(rpos, rpos + count);
    return std::make_shared<myShader>(rpos, colors, count);
}
*/


std::shared_ptr<GShader> GCreateBitmapShader(const GBitmap& bitmap, const GMatrix& localm, bool avg) {
    
    assert(bitmap.width() > 0);
    assert(bitmap.height() > 0);
    GMatrix m = localm; //* GMatrix::Scale(bitmap.width(), bitmap.height());

    GBitmap bm = bitmap;
    //bm.readFromFile("apps/spock.png");
    
    
    return std::make_shared<myShader>(bm, m, true);
    //return std::make_shared<myShader>(bm, m);
    
}