// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table; the attribute
// members are numbered because their meanings are not established.
class CBotCharacter {
  public:
    CBotCharacter();
    void Init();
    float attributes[7];   // +0x00; all set to one value by Init.
    unsigned char unknown1c[0x48 - 0x1c];
    float field48;         // +0x48
    int field4c;           // +0x4c
    int field50;           // +0x50
    float field54;         // +0x54
    float field58;         // +0x58
};

CBotCharacter::CBotCharacter() { Init(); }

void CBotCharacter::Init() {
    attributes[0] = 1.0f;
    attributes[1] = 1.0f;
    attributes[2] = 1.0f;
    attributes[3] = 1.0f;
    attributes[4] = 1.0f;
    attributes[5] = 1.0f;
    attributes[6] = 1.0f;
    field48 = 0.5f;
    field4c = 1;
    field50 = 1;
    field54 = 1.0f;
    field58 = 4.0f;
}
