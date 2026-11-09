// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "DrawContext.h"
#include "StaticMesh.h"
#include "IVolume.h"
#include "EAGLTransform.h"

static inline void Transform(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

static inline void GetSphereCenter(COORD3 &center, const StaticMeshPart &part) {
    center.x = part.center[0];
    center.y = part.center[1];
    center.z = part.center[2];
}

int CAnimObject::IsVisible(CDrawContext &context) const {
    if (alwaysVisible)
        return 1;
    if (localVolumeType3)
        worldVolumeType3->TransformedCopy(*localVolumeType3, transform);
    if (worldVolumeType3)
        return worldVolumeType3->TestVisibility(context);
    StaticMeshPart *part = mesh->part;
    COORD3 center;
    GetSphereCenter(center, *part);
    float radius = part->radius;
    CVector3 world(center.x, center.y, center.z);
    ::Transform(world, transform, world);
    return context.IsSphereVisible(*(const COORD3 *)&world, radius);
}
