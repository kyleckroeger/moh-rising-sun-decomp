// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp.
#include "StaticMesh.h"

void CStaticMesh::Init(void *newMesh, bool load) {
    mesh = (StaticMesh *)newMesh;
    animShapes = 0;
    unknown10 = -1;
    unknown14 = -1;
    if (load)
        Init();
    else
        InitAnimatedTexturesOnly();
}
