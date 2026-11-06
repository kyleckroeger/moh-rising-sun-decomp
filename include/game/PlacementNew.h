// AI-assisted reconstruction; the header name is a project choice. The one definition
// of global placement new, so that a file can include any headers that need it.
#ifndef GAME_PLACEMENTNEW_H
#define GAME_PLACEMENTNEW_H
inline void *operator new(unsigned int, void *p) { return p; }
#endif
