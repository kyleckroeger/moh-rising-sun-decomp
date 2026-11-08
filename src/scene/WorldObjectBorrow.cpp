// AI-assisted reconstruction from GR8E69. Part of the original world_object.cpp; member names are
// descriptive. Borrow lends up to four compartment-sized blocks and returns the lowest address.
#include "WorldObject.h"

void *CWorldObject::Borrow(int &size) {
    if (size == -1)
        size = m_sCompartmentSize * 4;
    if (size > m_sCompartmentSize * 4) {
        size = 0;
        return 0;
    }
    void **lowest = 0;
    int count = (size - 1) / m_sCompartmentSize + 1;
    for (int i = 0; i < count; i++) {
        void **block = blockPool.Take();
        if (!lowest || (int)lowest > (int)block)
            lowest = block;
        borrowedBlocks[i] = block;
    }
    return lowest;
}

void CWorldObject::Return() {
    for (int i = 0; i < 4; i++) {
        void **block = borrowedBlocks[i];
        if (block) {
            blockPool.Give(block);
            borrowedBlocks[i] = 0;
        }
    }
}
