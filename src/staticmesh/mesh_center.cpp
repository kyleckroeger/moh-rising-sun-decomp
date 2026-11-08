// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp.
#include "StaticMesh.h"

void CStaticMesh::GetCenter(CVector3 &center) const {
    const StaticMesh *m = mesh;
    if (m->part0) {
        const StaticMeshPart *part = m->part0;
        center.x = part->center[0];
        center.y = part->center[1];
        center.z = part->center[2];
    } else if (m->part1) {
        const StaticMeshPart *part = m->part1;
        center.x = part->center[0];
        center.y = part->center[1];
        center.z = part->center[2];
    }
}

