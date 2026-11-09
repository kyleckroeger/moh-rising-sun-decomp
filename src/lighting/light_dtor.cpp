// CLight's destructor and the update that follows a parent's transform.
#include "Light.h"

inline ISceneNode::~ISceneNode() {}

CLight::~CLight() {
}

static inline void PostMultiply(CMatrix &m, const CMatrix &other) {
    CMatrix::s_TempMat = m;
    m.Multiply(other, CMatrix::s_TempMat);
}

void CLight::BeginUpdate(float) {
    if (parent) {
        parent->GetTMLocalToWorld(localToWorld);
        PostMultiply(localToWorld, attachTransform);
    }
}
