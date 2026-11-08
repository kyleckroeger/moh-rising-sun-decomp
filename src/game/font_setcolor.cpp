// CFont::SetColor.
#include "Font.h"

void CFont::SetColor(CColor color) { handle->color = *(unsigned int *)&color; }
