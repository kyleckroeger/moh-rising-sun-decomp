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

- `Reset` calls `map<int, CPlayerWeaponObject *>::clear`, whose `_M_erase` is instantiated
  in the original. A source would need `include/stlport-containers` in its manifest.
- `Yaw` and `SetTMLocalToWorld` share an inline rotation. The closest source differs only by
  the scheduling of two address computations against the axis stores, so they remain context.
