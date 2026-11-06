# Weapons

`src/weapons/` reconstructs `CWeapon` functions from the pinned GR8E69 executable.
Claude Code assisted the analysis and verification. Medal of Honor: Frontline was
used as a semantic reference for control flow only; its layouts differ.

## Verified against GR8E69

- `CWeapon` calls `CStaticObject::BeginUpdate` and `CStaticObject::Draw` directly, and
  its constructor and destructor call the `CStaticObject` ones, so `CStaticObject` is a
  base. Further bases are not declared. The second table pointer at `+0x3c` and the
  subject and observer subobjects at `+0x374` and `+0x388` are not reconstructed.
- Offsets in `include/game/Weapon.h`: properties `+0x300`, reserve `+0x30c`, clip
  `+0x30e`, weapon class `+0x320`, position `+0x340`, flag word `+0x350`, muzzle flash
  `+0x360`, holder `+0x370`. `+0x308` is the value `RegisterWithAI` looks up in the
  level resource table.
- `Draw` skips drawing when the holder answers `AsPlayerObject`: the player draws its
  weapons through `CPlayerWeaponObject`. Frontline's reconstruction uses a different
  slot here and inferred the name `DrawsWeapon`.
- `AddAmo` starts a reload when the weapon held nothing, caps the reserve at the
  property word `+0x12`, and for weapon class 7 keeps the reserve one below that cap.
  `Shutdown` destroys the weapon script; Frontline's is empty.

## Inferred

Member names are descriptive. `SetAmmo`'s parameter meanings follow its stores.
`DoneReloading` reads the infinite-ammunition bit through a plain local, because the
extracted value is reused as the zero stored to the reserve.
`WeaponAIBehaviorRecord`, `WeaponAIEntry` and `TLTResource` in `WeaponAmmo.cpp` are
scoped views of the chain `RegisterWithAI` walks, and their names are not original.

## Not reconstructed

`__7CWeapon`, `_._7CWeapon`, `Init`, `Update` and `FireBullet` remain original context.

## Player weapon objects and factory

`CPlayerWeaponObject` forwards ammunition calls to the `CWeapon` at `+0x2ec`. `IsSelectable`
answers whether weapon class 7 has rounds left, otherwise bit 29 of the flags at `+0x3f0`.
`GetBulletEmitterMatrix` finds the joint `jo_fire` through the skeleton returned by slot 2
of the subobject at `+0x38` and uses `g_pStaticObjectMuzzleFlashPlayer`.
`CWeaponFactory::DestroyWeapon` calls `Shutdown` and then `MarkForDestruction(1)`.
`CWeaponFactory::Init` differs from its original by the scheduling of three argument
registers around the factory's `Init` call and remains original context.
`CPlayerWeaponObject::Shoot` matches but stays context: its double and float literals start
an 8-byte aligned `.rodata` section that ngcas pads past the string the next function owns.
