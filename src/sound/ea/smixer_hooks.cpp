// The start of the software mixer (smixer.c): SNDI_New takes cleared sound
// memory and SNDI_Delete frees it. The names come from the symbols.
void* SNDMEMI_allocz(int size);
void SNDMEMI_free(void* memory);

void* SNDI_New(unsigned int size) {
    return SNDMEMI_allocz(size);
}

void SNDI_Delete(void* memory) {
    SNDMEMI_free(memory);
}
