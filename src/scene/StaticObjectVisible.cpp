// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

int CStaticObject::IsVisible(CDrawContext &context) const {
    IVolume *volume = worldVolumeType3;
    int visible;
    if (!volume)
        visible = 1;
    else
        visible = volume->TestVisibility(context);
    return visible;
}

