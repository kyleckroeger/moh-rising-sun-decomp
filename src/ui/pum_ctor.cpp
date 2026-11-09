// PopUpMessageHandler's constructor loads the message table and the two fonts of the level, scales
// the first for split-screen modes, and loads the backing sprites; its destructor shuts the handler down.
// Part of the original pumhandler.cpp.
#include "Objectives.h"
#include "Font.h"
#include "Toilet.h"
#include "Screen.h"
#include "GameSettings.h"

extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }

class CSprite {
  public:
    enum ESpriteLoadType { SpriteLoadUnknown0 = 0, SpriteLoadUnknown1 };
    CSprite(const char *, ESpriteLoadType);
    ~CSprite();
    unsigned char data[36];
};

extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
extern int g_NumPlayers;
extern unsigned char maxPopupsPerViewport;
extern float MP_PUM_TEXT_SCALE_X_2P, MP_PUM_TEXT_SCALE_Y_2P, MP_PUM_TEXT_SCALE_X_4P, MP_PUM_TEXT_SCALE_Y_4P;

PopUpMessageHandler::PopUpMessageHandler(char *a, char *b, char *c) {
    tables[0].Load(a, b, c);
    LevelFileContentsStruct_ *fonts = TLT_FindNextResourceByType((TLTResourceID)25, 0);
    char *font0 = (char *)TLT_LoadFileFromLevelBigFile((const char *)fonts->data[27]->value, 0);
    char *font1 = (char *)TLT_LoadFileFromLevelBigFile((const char *)fonts->data[2]->value, 0);
    CScreen *screen = CScreen::m_spScreen;
    font644 = new ("source/UserInterface/pumhandler.cpp:107") CFont(font0);
    font644->SetColor(CColor(63, 63, 63, 255));
    font644->SetScale(CScreen::m_sWidthScale, CScreen::m_sHeightScale);
    if (screen)
        font644->SetDepth(screen->GetDepthBufferMax());
    if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
        switch (g_NumPlayers) {
        case 1:
            font644->SetScale(1.0f, 1.0f);
            break;
        case 2:
            if (g_bInMultiplayerMode)
                font644->SetScale(MP_PUM_TEXT_SCALE_X_4P, MP_PUM_TEXT_SCALE_Y_4P);
            else
                font644->SetScale(MP_PUM_TEXT_SCALE_X_2P, MP_PUM_TEXT_SCALE_Y_2P);
            break;
        case 3:
        case 4:
            font644->SetScale(MP_PUM_TEXT_SCALE_X_4P, MP_PUM_TEXT_SCALE_Y_4P);
            break;
        }
        for (int i = 3; i >= 0; i--)
            slots658[i] = -1;
    }
    float width;
    font644->GetRenderedSize("AWgjy", width, lineHeight);
    lineHeight += lineHeight * 0.05f;
    font648 = new ("source/UserInterface/pumhandler.cpp:148") CFont(font1);
    font648->SetColor(CColor(63, 63, 63, 128));
    font648->SetScale(CScreen::m_sWidthScale, CScreen::m_sHeightScale);
    if (screen)
        font648->SetDepth(screen->GetDepthBufferMax());
    if (!sprite64c)
        sprite64c = new ("source/UserInterface/pumhandler.cpp:158") CSprite("data\\popupBack.gsh", CSprite::SpriteLoadUnknown1);
    if (!sprite650)
        sprite650 = new ("source/UserInterface/pumhandler.cpp:164") CSprite("data\\popupSubtitleBack.gsh", CSprite::SpriteLoadUnknown1);
    settings654 = g_GameSettings.unknown_a8;
    if (g_bInMultiplayerMode && g_NumPlayers > 1)
        maxPopupsPerViewport = 2;
}

PopUpMessageHandler::~PopUpMessageHandler() { ShutdownPopUpMessageHandler(); }
