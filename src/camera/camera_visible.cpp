// A camera is never drawn or visible.
#include "Camera.h"
unsigned int CCamera::IsDrawEnabled() const { return 0; }
int CCamera::IsVisible(CDrawContext &) const { return 0; }
