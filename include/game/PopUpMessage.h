#ifndef POPUPMESSAGE_H
#define POPUPMESSAGE_H

// One on-screen message. Method names come from the symbol table; member names follow
// the accessors, and members without an accessor are named by what the code does.
class PopUpMessage {
public:
    PopUpMessage();
    ~PopUpMessage();
    void Reset();
    void Set_placement(unsigned char value);
    void Set_screenLocs(unsigned short x, unsigned short y);
    void Set_property(unsigned char value);
    void Set_onScreenTime(float value);
    void Set_textSize(unsigned char value);
    void Set_viewportID(short value);
    void Set_messageID(short value);
    void Set_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
    void Set_top(unsigned short value);
    void Set_bottom(unsigned short value);
    void ChangeAvailability(unsigned char value);
    void SetString(const char *text);
    unsigned char Available();
    unsigned char Get_placement();
    unsigned short Get_screenLocX();
    unsigned short Get_screenLocY();
    unsigned char Get_property();
    float Get_onScreenTime();
    short Get_viewportID();
    short Get_messageID();
    unsigned char Get_r();
    unsigned char Get_g();
    unsigned char Get_b();
    unsigned short Get_top();
    char *GetString();
    void MoveTop();
    void Scroll();
    void UpdateDisplayTime(float elapsed);

    unsigned char placement;
    unsigned short screenLocX;
    unsigned short screenLocY;
    unsigned char property;
    float onScreenTime;
    unsigned short textSize;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    unsigned short top;
    unsigned short bottom;
    unsigned short field16; // +0x16, set to 4 by Reset; meaning unknown
    unsigned char available;
    short viewportID;
    short messageID;
    char string[48];
};

#endif
