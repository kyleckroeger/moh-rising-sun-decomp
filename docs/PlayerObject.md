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

`LineAlmostCollided` in `CAnimatedPlayer` is an empty body; its `int` return type follows
the shared virtual declaration and the original returns nothing meaningful.

## Parked

- `GetCurrentBulletSprite`: the wielded-weapon selection loads into `r3` in the original
  and through `r9` in every tried source (12 instructions differ).
- `Yaw` and `SetTMLocalToWorld`: see above.

## Death sequence

`StartDeathSequence` and `UpdateDeathSequence` are reconstructed from the disc; Frontline
supplied the shape. The timer starts at 180 and counts down at +0xdc0. The multiplayer
branch of `UpdateDeathSequence` reads a default-constructed matrix that nothing fills in
this build: Frontline's `GetLookMatrix` call on the killer is gone from Rising Sun, so the
original reads uninitialized stack here, and the source reproduces that as it stands.
