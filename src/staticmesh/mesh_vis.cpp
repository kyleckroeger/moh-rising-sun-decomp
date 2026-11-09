// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp. A mesh is visible when either part's bounding sphere is.
#include "DrawContext.h"
#include "StaticMesh.h"
#include "EAGLTransform.h"

static inline void GetSphereCenter(COORD3 &center, const StaticMeshPart &part) {
    center.x = part.center[0];
    center.y = part.center[1];
    center.z = part.center[2];
}

int CStaticMesh::IsVisible(CDrawContext &context) const {
    bool visible = false;
    if (mesh) {
        StaticMeshPart *part = mesh->part0;
        if (part) {
            COORD3 center;
            GetSphereCenter(center, *part);
            bool hit = context.IsSphereVisible(center, part->radius) != 0;
            visible = hit;
        }
        if (mesh) {
            part = mesh->part1;
            if (part) {
                COORD3 center;
                GetSphereCenter(center, *part);
                visible = (visible != 0) | (context.IsSphereVisible(center, part->radius) != 0);
            }
        }
    }
    return visible;
}
