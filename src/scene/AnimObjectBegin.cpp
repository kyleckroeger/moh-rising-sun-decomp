// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }

void CAnimObject::BeginUpdate(float dt) {
    animRemaining = dt;
    animRate = 1.0f / dt;
    lightVolumes.Update(dt);
    previousPosition = transform.GetPos();
    yawCopy = yaw;
    // The skeleton's matrix table is at +0x34; the root joint's matrix is the second entry.
    CMatrix root;
    CopyMatrix(root, (*(const CMatrix **)(skeleton + 0x34))[1]);
    committedRotation.SetFromMatrix(root);
    committedOffset = CVector3(root.row[3].x, root.row[3].y, root.row[3].z);
    ((CAnimObjectInterface *)this)->UpdateAnimation(dt);
    if (attached || hurtlingBlocked) {
        velocity.Zero();
    } else if (hurtling) {
        if (gravity)
            velocity.z = 0.0f;
    } else if (gravity) {
        float z = velocity.z + dt * -0.002727675484493375f;
        velocity.z = z;
        if (!(__builtin_fabsf(z) <= 0.5f)) {
            if (!(z <= 0.0f))
                velocity.z = 0.5f;
            else
                velocity.z = -0.5f;
        }
        fallStarted = 1;
    }
}
