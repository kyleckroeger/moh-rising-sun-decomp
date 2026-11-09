// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::InvalidateSlotBackup() { slotBackupValid = 0; }
