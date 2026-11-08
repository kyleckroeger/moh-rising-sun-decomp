// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp.
#include "StaticMesh.h"

void CStaticMesh::EnableLighting(bool enable) {
    lighting = enable;
}

void CStaticMesh::EnableFogging(bool enable) {
    fogging = enable;
}

bool CStaticMesh::IsLightingEnabled() {
    return lighting;
}

bool CStaticMesh::IsFoggingEnabled() {
    return fogging;
}
