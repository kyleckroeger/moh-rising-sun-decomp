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
- The grenade-response `IsCatchable` helper reads the signed property halfword
  at `+0x62`. A nonzero value changes its side-projection bounds from (-0.1, 0.5)
  to (-0.3, 0.2). The header calls it `field_62`; its original meaning is unknown.
  See [grenade response queries](Bullets.md#grenade-response-queries).

## Inferred

Member names are descriptive. `SetAmmo`'s parameter meanings follow its stores.
`DoneReloading` reads the infinite-ammunition bit through a plain local, because the
extracted value is reused as the zero stored to the reserve.
`WeaponAIBehaviorRecord`, `WeaponAIEntry` and `TLTResource` in `WeaponAmmo.cpp` are
scoped views of the chain `RegisterWithAI` walks, and their names are not original.

## Not reconstructed

`__7CWeapon`, `_._7CWeapon`, `Init` and `FireBullet` remain original context.

## Player weapon objects and factory

`CPlayerWeaponObject` forwards ammunition calls to the `CWeapon` at `+0x2ec`. `IsSelectable`
answers whether weapon class 7 has rounds left, otherwise bit 29 of the flags at `+0x3f0`.
`GetBulletEmitterMatrix` finds the joint `jo_fire` through the skeleton returned by slot 2
of the subobject at `+0x38` and uses `g_pStaticObjectMuzzleFlashPlayer`.
`Init(int, int, CPlayerObject *)` creates the weapon through the factory and adds the object
to the scene. `BeginUpdate` follows the weapon's reserve ammunition in `+0x3f4` outside
multiplayer; while bit 30 of the flags is set it updates the animation and, unless the player
hides the weapon, the transforms, the weapon and the muzzle flash. `Draw` runs only for the
player's own view, honours the delay in `+0x3b0`, complains if the object at `+0x2d8` is null
and queues `DrawWeapon` in bin 11 with the matrix at `+0xd0` bound. Field names are descriptive.
`CWeaponFactory::DestroyWeapon` calls `Shutdown` and then `MarkForDestruction(1)`.
`CWeaponFactory::Init` differs from its original by the scheduling of three argument
registers around the factory's `Init` call and remains original context.
`CPlayerWeaponObject::Shoot` matches but stays context: its double and float literals start
an 8-byte aligned `.rodata` section that ngcas pads past the string the next function owns.

`CWeaponFactory::CreateWeapon` and `CreateWeaponByCRC` inline the object factory's allocator; its
warning text names it `CGameObjectFactory::AllocateGameObject`. The allocator walks a pool of
`objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE` strides for an entry whose id at `+0x08` is zero,
clears the storage and constructs the weapon in place with no null test. The entry's `+0x04` and
the first word of the view at weapon `+0x38` link entry and object. `CreateWeaponByCRC` falls back
to the first table entry for an unknown nonzero CRC and returns null for CRC zero. Field names are
descriptive.
