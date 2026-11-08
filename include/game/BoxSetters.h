#ifndef BOXSETTERS_H
#define BOXSETTERS_H
#include "Box.h"
// Inferred inline bodies for the setters CVolBox declares; the names come from Frontline. They
// live outside Box.h because that header's implementation unit emits every inline defined there.
inline void CVolBox::SetBasis(const CVector3 &x, const CVector3 &y, const CVector3 &z) {
    axisX = x;
    axisY = y;
    axisZ = z;
}
inline void CVolBox::SetWidth(float w) { halfSize[0] = w * 0.5f; }
inline void CVolBox::SetDepth(float d) { halfSize[1] = d * 0.5f; }
inline void CVolBox::SetHeight(float h) { halfSize[2] = h; }
inline void CVolBox::SetCenter(const CVector3 &c) { center = c; }
#endif
