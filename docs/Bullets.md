# Bullets

`src/bullets/` reconstructs 38 `CBullet`, `CProjectileBullet` and `CThrownBullet`
functions (672 executable bytes) from the
pinned GR8E69 executable. Claude Code assisted the analysis and verification.

## Declarations

The `CBullet` table at `0x802e9eb0` follows the [scene-node slot map](SceneNode.md)
for slots 1-88 and adds slots 89-102: `Init`, `Shutdown`, `GetUp`, `GetDamage`,
`GetScriptBulletType`, `GetFiredBy`, `AsThrown`, `AsProjectile`, `GetVelocity`,
`SetExclusionPair`, `Penetrate`, `SetDamage`, `SetBlastRadius` and
`SetExplosionParticleSystem`. `CProjectileBullet` (`0x802ea210`) adds
`SetDrawForward` and `CThrownBullet` (`0x802ea5a8`) adds `IsCollisionEnabled` at slot
103. `include/game/Bullet.h` declares `CBullet` under `#pragma interface` with a
scoped view: a collision ID at `+0x40` (`Shutdown` resets it to -1, and both derived
`GetCollisionId` overrides read it), the local-to-world matrix at `+0x70` (the axis
getters read its rows) and two floats at `+0xb0`/`+0xb4`. Return types follow the
same conventions as [SceneNode.md](SceneNode.md): `AsThrown`/`AsProjectile` return the
named classes; `GetFiredBy` returns `ISceneNode *`, inferred from its name and the
derived classes' pointer field; `GetScriptBulletType` is an `int` placeholder.

## Accepted fragments

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `bullet_base` | `Shutdown`, `Draw`, `BulletBaseDraw`, `IsDrawEnabled` | 80 | 8 bytes at `0x802a8c1c` |
| `bullet_defaults` | axis getters, `GetTMLocalToWorld`, `AsBullet`, and the base defaults of slots 92-102 | 440 | 20 bytes at `0x802a8c50` |

`Shutdown` sets `+0xb0` to 0, the collision ID to -1 and `+0xb4` to the largest
float, in that order. `Draw` calls the empty global `BulletBaseDraw`. The base
`GetDamage` returns 0, `GetScriptBulletType` -1, and the casts and `GetFiredBy`
null; the other defaults are empty.

## Derived accessors

`Init` stores its `ProjectileBulletProperties_struct` or `ThrownBulletProperties_struct`
argument at `+0xc8`; the scoped record views name only the fields read below.

| Manifest | Functions | Code bytes |
| --- | --- | ---: |
| `projectile_collision_id`, `thrown_collision_id` | `GetCollisionId` (both) return the collision ID at `+0x40` | 16 |
| `projectile_exclusion_pair` | `CProjectileBullet::SetExclusionPair` (empty) | 4 |
| `projectile_set_damage` | `SetDamage` writes record `+76` | 12 |
| `projectile_accessors` | `GetDamage` (record `+76`), `GetFiredBy` (`+0xe0`), `AsProjectile`, `SetDrawForward` (bit 28 of the word at `+0x10c`) | 40 |
| `thrown_setters` | `SetDamage` (record `+92`), `SetBlastRadius` (record `+96`), `SetExplosionParticleSystem` (word array at `+0x264` indexed by type) | 40 |
| `thrown_collision_light` | `IsCollisionEnabled` (returns 1), `GetAttachedLight` (`+0x254`) | 16 |
| `thrown_accessors` | `GetDamage` (record `+92`), `GetFiredBy` (`+0xe0`), `AsThrown` | 24 |

`+0xe0` is named from `GetFiredBy`. Both original `Init` routines pass its value
as the `ISceneNode` reference argument to `CScene::IncludeCollision`, supporting
the declared pointer type. The surrounding observer storage remains unknown. The
bounding-volume getters (embedded 80-byte volumes at `+0x110`/`+0x160` and
`+0x1a0`/`+0x1f0`) and `CThrownBullet::SetLightVolumeTransitionDuration` are not
reconstructed.

Maintainer review independently checked the thrown-bullet initializer at
`0x80150b24`: its default explosion setup writes CRCs to `+0x264`, `+0x268`,
`+0x26c` and `+0x270` (stores at `0x801519fc`, `0x80151a0c`, `0x80151a1c`
and `0x80151a2c`). The header therefore represents four observed particle-ID
slots, replacing the earlier one-element placeholder. This avoids indexing
beyond the declared array for those slots without claiming a complete object
size or recovering the enum member names. The setter retains the original
unchecked indexing; no new range check is introduced. This correction and its
independent GR8E69 verification used Codex.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces both
fragments, including their constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.

## Factory and later accessors

`CBulletFactory` holds the projectile and thrown property tables (96 and 104 bytes per
row, from the indexing) and three `CBulletFactoryHelper<T, P, E>` members, each 64 bytes:
projectiles, projectiles with collision id 3, and thrown bullets. The helper instantiations
are external here. `include/game/Bullet.h` names the row's CRC `+0x00`, weight `+0x04`,
sprite `+0x48`. Names not in a symbol, such as `mountedProjectiles`, are inferred.

`CProjectileBullet::OnCollision` sets `hitPending`, forwards to the other node's
`HandleBulletCollision`, and takes the collision time only if the handler cleared the bit.
`CThrownBullet::ApplyForceTo` scales the direction by the force, 1/512 and the row's
weight, and mirrors the result to `+0x150`. `Explode` writes the inverse of bit 27 to bit
28 of `+0x25c`. `IsVisible` calls `CVolFiber::TestVisibility` directly in both classes.

`CThrownObject` in `include/game/Thrown.h` is a scoped view with `CStaticObject` as its
base; `LimitSpeed` caps the speed at twice the world volume's radius while `+0x20c` is not
-1. Its source lives in `src/weapons/`.

## Explosions and projectile updates

`CExplosion` derives from `dwi::IVisitor<ISceneNode>` (symbols); its sphere is at
+0x10, then strength, damage, the script bullet type, the source node and two flags
(+0x30 to +0x44). `Visit` matches at 1,996 bytes. It skips animated players and player
weapon objects, scales damage by a falloff of `1 - (distance - 1) / radius` clamped to
0.5, gives a direct-hit node full damage in multiplayer (`g_pDirectHitNode`), and sends
events 178 and 40 with an eight-word `BS_STRUCT_HitByBulletInfo_struct`. Only the field
meanings are inferred; event numbers and constants are read from the code. Unlike
Frontline, Rising Sun applies no explosion push here: the original still calls
`AsPlayerObject` and discards the result at that point.

Parked in `scratch/par/player/Explosion.cpp`: the constructor (two argument moves
swapped), `ShouldHeightCheckBeFlipped` (one load-immediate position), `TestRay` and
`CheckForObstructions`.

`SetScaled` and `SetSum` in `ProjectileUpdate.cpp` are descriptive inline helpers; the
original store and reload order requires a by-reference result, but their names and
their existence as functions are inferred.

## Thrown object updates

`CThrownObject::OnCollision`, `UpdateCollisionVolumes`, `Draw`, `BeginUpdate`,
`AttemptUpdate`, `CommitUpdate` and `HandleBulletCollision` are reconstructed. The field
names at +0x300 to +0x3a0 and the bit names in the flags word at +0x394 are Frontline's
readings or inferred. Scene node slots are checked against the disc: where Frontline
tests `AsWorldObject`/`AsSoldierObject` for script state 1, Rising Sun tests
`AsAnimObject`/`AsPlayerObject`. Events are 224 and 225, against Frontline's 231 and 232.
`Constrain` stays parked at `scratch/par/player/ThrownConstrain_parked.cpp` (reload of the
reflected normal differs, 176 of 204 instructions aligned).

`CThrownBullet::Constrain` (bounce, friction and the optional keep-speed cheat word at
`g_cheats` +0x1c, then a clamped time-of-impact position) and `CreateExplosionEffect` are
reconstructed. `CExplosion` is 0x50 bytes: slot reuse across `CThrownBullet::CommitUpdate`
fixes it. `CommitUpdate` itself is parked at
`scratch/par/player/ThrownBulletCommit_parked.cpp`, one instruction out of position.

`CBulletFactory::LoadBulletGUISprites` allocates through `DWI_alloc` with the labels
`source/weapons/bulletfactory.cpp:567` and `:603`, which record the original path and
line. Projectile sprite names sit at +0x27 of each 96-byte record and thrown ones at +0x27
of each 104-byte record. `DeleteBulletGUISprites` is parked at
`scratch/par/player/BulletSpritesDelete_parked.cpp`: only two loop registers are swapped.

`CWeaponFactory::LoadWeaponGUISprites` builds `data\<name>` in a stack buffer and then
loads the sprite from the bare name, so the built path goes unused in this build. The
source keeps that as it stands; the icon field names at +0x38 and +0x58 are inferred.
