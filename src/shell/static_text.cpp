// CStaticScreen::DrawLoadingText draws the level's loading message for three frames with its own font.
// Part of the original StaticScreen.cpp.
#include "Screen.h"
#include "DrawContext.h"
#include "Font.h"
#include "Objectives.h"
#include "Toilet.h"
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
void WaitScreen();
class CStaticScreen {
  public:
    virtual ~CStaticScreen();
    void DrawLoadingText();
    CScreen *screen; // +0x00
};
void CStaticScreen::DrawLoadingText() {
    char *data = (char *)TLT_LoadFileFromLevelBigFile("objFont.gfn", 0);
    CFont *font = new ("source/shell/StaticScreen.cpp:587") CFont(data);
    font->SetColor(CColor(95, 95, 95, 255));
    font->SetScale(CScreen::m_sWidthScale, CScreen::m_sHeightScale);
    const char *text = g_pPopUpMessageHandler->tables[0].GetString(1);
    CDrawContext *context = screen->GetOrthographicDrawContext();
    for (int i = 3; i > 0; i--) {
        context->Begin(1);
        font->DrawText(55.0f, CScreen::m_sHeightScale * 390.0f, text);
        context->End();
        WaitScreen();
    }
    delete font;
}
