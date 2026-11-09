// AI-assisted reconstruction from GR8E69. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

static inline void Transform(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

void CAnimObject::MoveTo(const CVector3 &target, float speed) {
    if (hurtlingState)
        return;
    if (allowAnimationTranslation) {
        CMatrix rotation;
        CMatrix *r = &rotation;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        rotation.BuildRotZ(yaw);
        CVector3 world;
        ::Transform(world, *r, target);
        if (gravity) {
            velocity.x = world.x;
            velocity.y = world.y;
        } else {
            velocity = world;
        }
    }
    yawRate = speed;
}
