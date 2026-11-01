// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
extern BSClass_struct *g_pcClassList;
int BSFileGetMemoryRequirements() { return 0; }
int BSInitFile() { return 0; }
int BSEndFile() { g_pcClassList = 0; return 0; }
