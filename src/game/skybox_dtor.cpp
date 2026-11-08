// CSkyBox's destructor, which holds the class table, and its visibility queries.
#include "SkyBox.h"

// The original destructor inlines ~ISceneNode, whose out-of-line copy is a separate weak
// function in the image; the shared header declares it out of line.
inline ISceneNode::~ISceneNode() {}

CSkyBox::~CSkyBox() {
    screen->SetClear(true);
}

int CSkyBox::IsVisible(CDrawContext &) const {
    return 1;
}

unsigned int CSkyBox::IsDrawEnabled() const {
    return enabled;
}
