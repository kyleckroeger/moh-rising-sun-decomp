# Front-end and game settings

Rising Sun code with no Frontline counterpart. The evidence is the GR8E69 disassembly,
its symbol table and the existing headers. Exact native output is strong evidence for this target;
it does not establish every original type or statement.

## CGameSettings and its saved data

Verified by matching code: `CGameSettings` points at a `SCoreGameSettings` block of 0x321ec
bytes. The constructor compares that with `GameSettingReserve`, an absolute symbol of 0x321f8
in the linked image, and `Init` prints both. The block holds a language word,
four `SPlayerProfile` records of 0xc598 bytes at `+4`, the current `SSaveData` at
`+0x31664`, a separate `SPlayerStats` at `+0x31fb8`, the multiplayer player entries,
death-match statistics and a table mapping players to profile indices at `+0x321e0`.
An `SSaveData` is 0x954 bytes and each profile holds twenty at `+0x6c`. Struct tags come
from method symbols; `SPlayerStats`' four counters are named by the `Set_` methods.

Inferred: other member names describe what the code does with them. `SSaveData`'s
closing fields are called a time stamp only because their widths (16, 4, 6, 5, 6 and 6
bits) fit one. `inCoop` and `inMultiplayer` are two `g_Shell` flags: `UpdateUnplugged` chooses the controller
checks by them, and `CGameSettings` tests them alongside `g_bCoopMultiplayer` and
`g_bInMultiplayerMode`. The array bound of 20 saves is from the
space between `+0x6c` and the next known field.

Runtime remains untested.

Each accepted fragment shares `GameSettingsViews.h`, whose views of the player, shell and
pause objects hold only the members these methods touch.

Not yet exact: `SetIfCurrentWeaponSelectable` (5 differences), `ActivateCheatNamed_All`,
`ActivateCheatNamed_Unlock` (register allocation only), `PerformSavePointResponse` (one
threaded branch), `Set_currentDifficulty` (two scheduled instructions), `ResetDMStats` (an
unreduced multiply in its loop) and `SCoreGameSettings::TotalReset`, whose loop treats the word
before each player's data as belonging to the entry being set up. The landed
`SMPPlayerSlot` view places that word first because `Respawn` and `SetHealthMax` compile
to that form; the two views have not been reconciled.

## FEng library

Rising Sun's front end runs on EA's FEng library. Source paths in allocation labels name its
files (`source/FEngine/FEWideString.cpp`), and symbols give the classes and methods. Verified by
matching code: `FEMinNode` holds two links and its table at `+8`; unlinking writes `0xabadcafe`
into both links. `FEMinList`, `FERefList`, `FEPackageList`, `FEMsgTargetList`, `FEJoyPad`,
`FEMouse`, `FEColor` and `FEWideString` are as in `FEngLists.h` and `FEngStrings.h` or their
source files. Inferred: member names, and that the fields shown are all a class holds.
`FEMinNode`'s destructor has a weak out-of-line copy in the image, so the real class defines it
inline; these units only call it through the table, so the declaration is enough for them.

`FEngine` keeps three lists at `+0xf0`, `+0x100` and `+0x110`, a stack of sixteen package-name
markers and a ring of thirty-two remembered package buttons. `PackageHandler` holds its table at
`+0x4c`. Its third `SetObjectString` overload takes a pointer to a type mangled `Uw`, for which no
spelling has been found, so that overload is unreconstructed, and the two others stay out because
they share a string with `EnableButton` across it.

`CShellMovieSettings` holds the `game.cfg` movie paths, named by their keys: four types of eleven
entries of three 64-byte strings, a demo entry and a credits entry, starting at `+0x84`. It ends at
`+0x2304`, which fixes the position of `ShellManager`'s following members.

`FEObject`'s flag word at `+0x1c`, data block at `+0x2c`, response and script lists and current
script at `+0x54` are verified by its methods and `PackageHandler`'s. Its data comes from the file-local
pool `ObjDataPool`.

Not yet exact: `Close` (FEObject.cpp, a float range test), `GetStringLength` (register allocation), `FEColor::operator-` (load order),
`ShellManager::RenderObjectList`, `RecallPackageMarker`, `RecordPackageMarker` and the
`PackageHandler` object-list index functions.
