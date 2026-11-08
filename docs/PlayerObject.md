# Player object

`src/player/Player*.cpp` reconstructs `CPlayerObject` transform and query functions
from the pinned GR8E69 executable. Claude Code assisted the analysis and verification.
Medal of Honor: Frontline was a semantic reference; the layouts differ.

`CPlayerObject` remains a member-only view in `include/game/CameraShake.h`. Member names
are descriptive. Verified offsets: local-to-world matrix `+0x50`, camera matrix `+0x90`,
bounds `+0x1e0`, velocity `+0x230`, force `+0x250`, position `+0x260`, yaw `+0x274`,
pitch `+0x278`, collision id `+0xfc0`. `ApplyForceTo` scales the force by `1/512`
and sets bit 18 of the flag word at `+0xd44`.

## Code-generation notes

- `CVector3::Zero` is the chained `x = y = z = 0.0f`, which gives the original store
  order. The member name is descriptive.
- `GetCentroid` reads the bounds through a local pointer and accessors that return by
  value. A temporary from an inline function is stored directly through `r1`, where a
  directly constructed local goes through its address register. `Bounds` and its accessors
  are inferred from that shape.

## Not reconstructed

- `Yaw` and `SetTMLocalToWorld` share an inline rotation. The closest source differs only by
  the scheduling of two address computations against the axis stores, so they remain context.

## Weapon map, current weapon and animated player

`weaponMap` at `+0xe60` is a `map<int, CPlayerWeaponObject *>`; `SetScript`, the light
volume forwards, `GetWeaponByCRC`, `Reset` and `ReInit` use its real STLport inlines.
The view keeps raw storage so other users of `CameraShake.h` need no STL headers, and
those units add `include/stlport-runtime` and `include/stlport-containers`.
`GetCurrentWeapon` selects `mountedWeapon` at `+0xe98` while bit 15 of `+0xd44` is set,
otherwise the wielded weapon object's `CWeapon`. `SetZoom` clamps to 5..35 and passes the
result to `TransitionFOV` with rate 2100; a mounted gun's flag and zoom value lower the
limit. `CPlayerObject::TargetFind` at `+0x1ca0` has an inline disable, whose name is
descriptive. `CAnimatedPlayer` is a separate member-only view; `Draw` skips the owning
player's view by comparing the word at the player's `+0x10d8`.

`CAnimatedPlayer::BeginUpdate` records the previous position, yaw, root-joint rotation and
position and clears the velocity. `AttemptUpdate` blends toward the joint matrix's rotation
and position by `dt`, moves the transform unless `attached` (`+0x238`) is set, updates the
bounding volume and collision-sort extents, clears three record words and copies the muzzle
state to the held weapon. Rising Sun's yaw handling reduces to zero here. The field names
follow Frontline where marked in the header and are otherwise descriptive. `PostUpdate` turns
`flocoHeading` toward the player's heading, raising script events 299 and 300 as the turn
starts and ends, and rebuilds the transform. `HandleBulletCollision` spawns the `EnemyHit`
effect aligned to the contact normal for projectile bullets and passes the hit to the player.

`LineAlmostCollided` in `CAnimatedPlayer` is an empty body; its `int` return type follows
the shared virtual declaration and the original returns nothing meaningful.

`HandleBulletCollision` on the player ignores bullets while bit 29 of the flags is set; for
projectiles and grenade types 4 and 7 it fills a 32-byte hit event (direction by the two dot
products at 0.7071), applies invincibility and the one-hit-kills cheat, in multiplayer team
protection and the contact's location (3x damage for locations 0 and 1), then raises script
event 40, drives rumble and plays the impact sound. `SetMPShotLocation` still computes the
location group it once recorded; the switch has no effect in this build, as in the original.

## Parked

- `Yaw` and `SetTMLocalToWorld`: see above.

## Death sequence

`StartDeathSequence` and `UpdateDeathSequence` are reconstructed from the disc; Frontline
supplied the shape. The timer starts at 180 and counts down at +0xdc0. The multiplayer
branch of `UpdateDeathSequence` reads a default-constructed matrix that nothing fills in
this build: Frontline's `GetLookMatrix` call on the killer is gone from Rising Sun, so the
original reads uninitialized stack here, and the source reproduces that as it stands.

## Respawn

`Respawn` restores the capsule and the box at `+0xe0` (`boxStorage`, kept as raw storage),
clears the per-life state, reinitializes from the player's `SMPPlayerData` and restarts the
player. `SMPPlayerData` is named by `InitMultiplayer`'s symbol; its 20-byte slot spacing is
observed and `restartIndex` is descriptive. `SetBasis`, `SetWidth`, `SetDepth`, `SetHeight`
and `SetCenter` on `CVolBox` are inferred inlines. A previous weapon no longer current after
reinitialization is cycled back to by its CRC; `fireDelay` comes from `RESPAWN_FIRE_DELAY`.

## Path movement, constraints and mounted positions

`UpdateMovePath` advances `pathParameter` while bit 19 of the path flags is set, evaluates
`movePath` at the half step (or integrates the velocity without a path), derives the velocity
from the new position and builds an orientation from `lookPath`'s tangent or next point.
`UpdateCollisionVolumes` refreshes the world capsule and collision-sort extents. `Constrain`
returns 1 at once while bit 14 of the flags, melee immunity or `mmgBlocked` is set; otherwise
it runs `SatisfyConstraints`, records failure in bit 20, limits the position change to 0.3 per
frame and recomputes the velocity (cleared on failure). `SetPlayersMountedMGPositions` derives
the player's transform, camera and mounted gun from the mount matrix through the offsets at
`+0xea0`, `+0xee0` and `+0xf60`. `SetHealthMax` copies `SMPPlayerData::maxHealth` into the
script data's health; `SetFinishedLevelData` records a won level. `AimAdjust::Enable` and
both `LookAtTarget::Enable` overloads clear or set the observed subject; their layouts are
observed and `time`, `target` and `active` are descriptive. `PostUpdate` repositions a mounted
gun, updates it, alternates the side bits in `mountedShotSide` and ejects a shell when bit 19 of
the flags is set.

## Save data and the weapon ring

`StoreSaveData` and `RestoreSaveData` copy health, the current weapon, each map weapon's
ammunition and selectability and the thrown weapon's ammunition to and from the game settings.
`CPlayerWeaponObject::IsSelectable` returns `bool`: `StoreSaveData` passes the result to a `bool`
parameter without normalizing it. `AddPlayerWeapon` allocates a weapon object once per CRC,
gives it the box volumes at `+0xe0` and `+0x130` and inserts it into the ring at `weaponList`
in ascending `weaponType`. `CCameraShake::Update` ramps toward the target and restarts with the
fade time when the duration passes. `CCameraShake::SetShake` (four arguments) and
`HitPickup` have matching code but stay context: each `.rodata` holds a double literal, and the
assembler's 8-byte padding of that section cannot verify without neighbouring functions' literals.
