# AI behaviors

The soldier's high-level behaviors derive from `CBehavior`. Every class name, method
name and table comes from the symbol table; member names are inferred from use. The
functions are fragments of the original `Behavior.cpp`, `SwordBehavior.cpp`,
`PistolBehavior.cpp`, `RifleBehavior.cpp`, `NullWeaponBehavior.cpp` and
`PolemineBehavior.cpp`, compiled with the standard game profile and verified byte for
byte, with their native tables and literal pools. There is no Frontline counterpart.

## Verified

- `CBehavior` holds the owning AI object at `+0`, a frame count at `+4` and the enable
  word at `+8`; its table pointer follows at `+0xc`. Its virtual slots are declared in table
  order in `AITargeting.h`. `Update` adds its frame argument and calls
  `UpdateHeuristicScore` when forced or when the count passes
  `GetHeuristicUpdateFrequency`.
- The weapon behaviors add slot 14, `SetWeaponScript`. All five update their heuristic
  every 60 frames and report whether the score changed. `GetType` returns 6 for the
  pistol, 7 for the rifle and the polemine, 8 for the sword and 9 for the null weapon.
- `StartBehavior` triggers event 269 on the weapon script and event 336 on the owner's
  script. `StopBehavior` triggers 271 when its argument is true and 270 otherwise.
- `ChooseSwordOrPistol` returns 1 for the sword when only the sword is usable, 0 when
  only the pistol is, and otherwise 1 when the target is within 10 units (squared
  distance below 100). It tests the embedded targeting module's address for null, as
  the original does.

## Inferred

- The names `aiObject`, `framesSinceUpdate`, `heuristicScore`, `weaponScript` and
  `swordDrawn`, and the description of the event arguments.
- `highLevelState` at `+0x10` of the pistol, rifle and null weapon: the constructor
  sets it to 4 and it is passed where the sword passes the AI object's high-level state.
- The `ECapability` enumerators are numbered only; what each capability means is not
  known. The enum's symbol-table tag is original.
- The inline-virtual methods that the original emits at the end of each file (`GetType`,
  `GetScriptObject`) are written as ordinary out-of-line definitions in that order.
