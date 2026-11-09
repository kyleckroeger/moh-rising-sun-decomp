// CViewport: a rectangle in normalized screen coordinates and its cached clip-to-view matrix.
#include "CMatrix.h"

class CViewport {
public:
    void SetRect(float left, float top, float right, float bottom);
    const CMatrix &GetTMClipToView() const;

    float left;
    float top;
    float right;
    float bottom;
    float width;
    float height;
    unsigned char unknown18[8];
    CMatrix clipToView;
    int clipToViewValid;
    int field64;
};

void CViewport::SetRect(float newLeft, float newTop, float newRight, float newBottom) {
    left = newLeft;
    top = newTop;
    bottom = newBottom;
    right = newRight;
    width = newRight - newLeft;
    height = newBottom - newTop;
    clipToViewValid = 0;
    field64 = 1;
}

const CMatrix &CViewport::GetTMClipToView() const {
    if (!clipToViewValid) {
        CVector3 scale(right - left, bottom - top, 1.0f);
        CVector3 offset(right + left - 1.0f, 1.0f - (bottom + top), 0.0f);
        CViewport *self = const_cast<CViewport *>(this);
        self->clipToView.BuildScale(scale);
        self->clipToView.SetPos(offset);
        self->clipToViewValid = 1;
    }
    return clipToView;
}
