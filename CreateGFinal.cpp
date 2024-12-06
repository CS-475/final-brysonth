#include "myCanvas.h"
#include "include/GCanvas.h"
#include <memory>
#include "include/GFinal.h"

std::unique_ptr<GFinal> GCreateFinal() {
    return std::make_unique<GFinal>();
    //return std::unique_ptr<GFinal_DEFINED>
}