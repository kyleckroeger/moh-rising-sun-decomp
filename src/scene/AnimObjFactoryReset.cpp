// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AnimObjFactory.cpp; member names are descriptive.
class CGameObjectFactory {
  public:
    void Reset();
};
class CAnimObjectFactory {
  public:
    void Reset();
    unsigned char unknown_0[0x10];
    // Four factories spaced 32 bytes apart; only their Reset is called here.
    unsigned char factoryA[0x20], factoryB[0x20], factoryC[0x20], factoryD[0x20];
};

void CAnimObjectFactory::Reset() {
    ((CGameObjectFactory *)factoryA)->Reset();
    ((CGameObjectFactory *)factoryB)->Reset();
    ((CGameObjectFactory *)factoryC)->Reset();
    ((CGameObjectFactory *)factoryD)->Reset();
}
