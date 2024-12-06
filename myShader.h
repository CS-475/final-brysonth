#ifndef MY_SHADER_DEFINED
#define MY_SHADER_DEFINED

#include "include/GShader.h"
#include "include/GPixel.h"
#include "include/GBitmap.h"
#include "include/GMatrix.h"
#include "myShader.h"

class myShader : public GShader {
public:
    myShader(const GBitmap& bitmap, const GMatrix& localMatrix);
    myShader(GPoint p0, GPoint p1, std::vector<GColor> colors, int count);
    myShader(const GBitmap& bitmap, const GMatrix& localMatrix, GTileMode tilemode);
    myShader(GPoint p0, GPoint p1, std::vector<GColor> colors, int count, GTileMode tilemode);
    myShader(GPoint p0, GPoint p1, const GColor color1, const GColor color2);

    //myShader(std::vector<GPoint> rpos, std::vector<GColor> colors, int count);
    myShader(const GBitmap& bitmap, const GMatrix& localMatrix, bool avg);


    

    bool isOpaque() override;
    bool setContext(const GMatrix& ctm) override;
    void shadeRow(int x, int y, int count, GPixel row[]) override;

    void setBitmap(const GBitmap& bitmap);
    void setLocalMatrix(const GMatrix& localMatrix) ;

    const GBitmap& getBitmap();
    const GMatrix& getLocalMatrix();


    
private:
    // Add any private member variables you need
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

#endif // MY_SHADER_DEFINED