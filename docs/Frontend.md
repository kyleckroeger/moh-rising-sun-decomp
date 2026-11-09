# Front-end and game settings

Rising Sun code with no Frontline counterpart. The evidence is the GR8E69 disassembly,
its symbol table and the existing headers. Exact native output is strong evidence for this target;
it does not establish every original type or statement.

## CGameSettings and its saved data

Verified by matching code: `CGameSettings` points at a `SCoreGameSettings` block of 0x321f8
bytes (the size its constructor compares against the loader's reservation). The block holds a language word,
four `SPlayerProfile` records of 0xc598 bytes at `+4`, the current `SSaveData` at
`+0x31664`, a separate `SPlayerStats` at `+0x31fb8`, the multiplayer player entries,
death-match statistics and a table mapping players to profile indices at `+0x321e0`.
An `SSaveData` is 0x954 bytes and each profile holds twenty at `+0x6c`. Struct tags come
from method symbols; `SPlayerStats`' four counters are named by the `Set_` methods.

Inferred: other member names describe what the code does with them. `SSaveData`'s
closing fields are called a time stamp only because their widths (16, 4, 6, 5, 6 and 6
bits) fit one. `coopA` and `coopB` are two `g_Shell` flags tested alongside
`g_bCoopMultiplayer` and `g_bInMultiplayerMode`. The array bound of 20 saves is from the
space between `+0x6c` and the next known field.

Runtime remains untested.

Each accepted fragment shares `GameSettingsViews.h`, whose views of the player, shell and
pause objects hold only the members these methods touch.

Not yet exact: `SetIfCurrentWeaponSelectable` (5 differences), `ActivateCheatNamed_All`,
`ActivateCheatNamed_Unlock` (register allocation only), `PerformSavePointResponse` (one
threaded branch), `Set_currentDifficulty` (two scheduled instructions).
