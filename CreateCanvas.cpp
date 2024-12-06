#include "myCanvas.h"
#include "include/GCanvas.h"
#include <memory>

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap& device) {
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}