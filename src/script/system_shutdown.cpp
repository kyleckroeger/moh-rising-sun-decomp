// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "MemStack.h"

extern void BSFreeAllBSObject();
extern void ShutdownObjectCreators();
extern int BSCleanUpBIFuncs();
extern void BSEndTimer();
extern void BSEndSchedule();
extern int BSEndMessage();
extern int BSEndEvent();
extern int BSEndMachine();
extern int BSEndFile();
extern void FreePropertyMemory();
extern "C" int DWI_free(void *);
extern unsigned char *g_pScriptMemory;
extern int g_memOffset, g_memSize;
extern MemStack *g_pScriptMemStack;

void BSUtilShutdownScriptingSystem() {
    BSFreeAllBSObject();
    ShutdownObjectCreators();
    BSCleanUpBIFuncs();
    BSEndTimer();
    BSEndSchedule();
    BSEndMessage();
    BSEndEvent();
    BSEndMachine();
    BSEndFile();
    FreePropertyMemory();
    DWI_free(g_pScriptMemory);
    g_pScriptMemory = 0;
    g_memOffset = 0;
    g_memSize = 0;
    delete g_pScriptMemStack;
    g_pScriptMemStack = 0;
}
