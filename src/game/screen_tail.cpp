// CScreen's clear state and the depth buffer's largest value.
#include "Screen.h"
void CScreen::SetClearColor(const CColor &color) { clearColor = *(const unsigned int *)&color; }
int CScreen::GetDepthBufferMax() const {
    switch (depthFormat) {
    case 0:
        return 0x7fff;
    case 1:
        return 0x7fffff;
    case 2:
        return 0x7fffffff;
    }
    return 0;
}
int CScreen::GetClear() const { return clear; }
const CColor &CScreen::GetClearColor() const { return *(const CColor *)&clearColor; }
void CScreen::Clear() {}
