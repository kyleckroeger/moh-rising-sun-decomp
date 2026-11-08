// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AnimObjFactory.cpp; member names are descriptive.
extern "C" void *DWI_allocalign(const char *, unsigned int, int, int);
extern "C" void DWI_free(void *);

class CGameObjectFactory {
  public:
    CGameObjectFactory();
    unsigned char storage[32];
};
// The block at +0x90 allocates 128 pointers up front.
struct AnimObjectFactoryBlock {
    void **entries; // +0x00
    int value04;    // +0x04
    int value08;    // +0x08
    int capacity;   // +0x0c
    int growth;     // +0x10
    int value14;    // +0x14
};
class CAnimObjectFactory {
  public:
    CAnimObjectFactory();
    void *definitions; // +0x00
    int definitionCount; // +0x04
    int unknown08[2];
    CGameObjectFactory factoryA, factoryB, factoryC, factoryD; // +0x10
    AnimObjectFactoryBlock block; // +0x90
};

CAnimObjectFactory::CAnimObjectFactory() : definitions(0), definitionCount(0) {
    AnimObjectFactoryBlock *b = &block;
    b->entries = 0;
    b->value04 = 0;
    b->value08 = 0;
    b->capacity = 0;
    b->value14 = -1;
    b->growth = 1;
    if (b->entries)
        DWI_free(b->entries);
    b->entries = (void **)DWI_allocalign("source/tl/dwi_memory.h:56", 128 * 4, 16, 1024);
    b->capacity = 128;
    b->growth = 1;
}
