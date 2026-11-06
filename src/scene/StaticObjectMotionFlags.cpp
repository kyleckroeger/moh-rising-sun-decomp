// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CStaticObject::StopMotionPlayback() { flags.motionPlaying = 0; }

void CStaticObject::ResumeMotionPlayback() { flags.motionPlaying = 1; }
