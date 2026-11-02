// Fragment of box.cpp: Create allocates a default-constructed box.
#include "Box.h"

IVolume *CVolBox::Create() const { return new CVolBox; }
