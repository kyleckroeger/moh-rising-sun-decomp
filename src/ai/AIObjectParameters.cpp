// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AIObject.cpp. The class name is from the symbol table; the
// members are named by offset because no reader of them is matched yet.
class CAIObjectParameters {
  public:
    CAIObjectParameters();
    ~CAIObjectParameters();
    float value00; // +0x00
    float value04; // +0x04
    float value08; // +0x08
    float value0c; // +0x0c
    unsigned char unknown10[0x18 - 0x10];
    float value18; // +0x18
};

CAIObjectParameters::CAIObjectParameters() {
    value00 = 0.4f;
    value04 = 0.0872664749622345f;
    value08 = 2.0f;
    value0c = 2.0f;
    value18 = 20.0f;
}

CAIObjectParameters::~CAIObjectParameters() {}
