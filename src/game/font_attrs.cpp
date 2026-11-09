// CFont's depth, scale and justification accessors and the rendered-size query.
#include "Font.h"

// The depth argument is not used.
void CFont::SetDepth(unsigned int depth) { handle->depth = 0.0f; }

void CFont::GetScale(float &x, float &y) {
    x = handle->scaleX;
    y = handle->scaleY;
}

void CFont::SetScale(float x, float y) {
    handle->scaleX = x;
    handle->scaleY = y;
}

void CFont::SetJustification(unsigned int value) { justification = value; }

unsigned int CFont::GetJustification() { return justification; }

void CFont::GetRenderedSize(const char *text, float &width, float &height) {
    float left = 0.0f, top = 0.0f;
    FONT_getrecta(handle, (const unsigned char *)text, &left, &top, &width, &height);
    width += left;
    height += top;
}
