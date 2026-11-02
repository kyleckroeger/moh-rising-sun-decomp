# Collision volumes

`src/geometry/` reconstructs the double-dispatch layer of the collision volumes, all
of `csg_volume.cpp`, most of `collision.cpp` and `collisionvolume.cpp` from the pinned
GR8E69 executable. Claude Code assisted the reconstruction. The user's
Medal of Honor: Frontline decompilation was a semantic reference; every function was
compiled with ProDG 3.8.1 and compared with the original instructions, and no runtime
testing has been performed.

## Evidence

- **Virtual order.** `_vt.7IVolume` names all 19 virtual functions, and every subclass
  table adds `TestCollision(const CCSGVolume &)` after them. A table entry that still
  names `IVolume` shows the subclass does not override it. Emitted tables are verified
  byte for byte.
- **Return type.** `TestCollisionWithSubVolumes__H1Z7CVolBox_...` records `EClsnState`
  as the template's return type. Its enumerators are not recovered; the sphere tests
  return 1 when apart and 2 on contact.
- **Inheritance.** `CAnimatedVolume` keeps `CVolSphere`'s visibility, extents and
  radius entries, and spheres forward animated volumes unadjusted, so `CVolSphere` is
  a base at offset zero.

## Inferred names and storage

`lineTest` (the top bit of the word at `CCollision+0x10`), `result` (`+0x18`), the
member names of `CVolSphere` and `CWorldVolume` and the header names are descriptive
project choices. Layouts differ from Frontline: the Rising Sun result word is at
`+0x18`, and `TestCollision(const CVector3 &)` takes its point by reference.

## Emission

Fiber, capsule, CSG and CDB object destructors sit among the other methods of their
files, so they are out of line and are each class's key function: the unit defining
one also emits the class table. `CVolSphere` and `CVolBox` destructors follow the last
regular method, with the sphere's radius accessors, which is how GCC 2 emits inline
virtual functions along with a table whose key function is `Create`. Those functions
can only be emitted by a source that also defines `Create`, so they wait for the rest
of those files. `CLight::IsVisible` inlines the sphere destructor, confirming that it
is inline. Fragment files here are project boundaries, not original ones.

A class's inline default constructor is emitted with its table too. The original link
strips it, but its literal stays: the word at `0x802a8490` between the fiber line
constructor's and `Create`'s literals belongs to the fiber's. The fiber manifest records
it as a discarded function, as `strip_unused` documents in [Lua.md](Lua.md).

## Contacts and the test driver

`CCollision` is 36 bytes. Contacts come from `CCollision::ms_contactPool`, over the 300
48-byte records of `g_aClsnContacts`, and form a list through `+44`. `AddContact` adds
the word at `+20` to the separation; copying and merging set that word to zero first so
the stored values are not adjusted twice. `Test` pads both volumes' radii by
`LARGEST_MOVE_PER_TICK`, runs the test and restores them, and sets the function-local
`isCSFWarningEnabled.6234` after a checked test. `SClsnContactPool` and its inline
helpers are project names: no symbol records the pool's type.

## Not reconstructed

`CCollisionVolume::~CCollisionVolume` inlines the `ISceneNode` destructor, which
`SceneNode.h` declares out of line, and stays original context. The sphere and box
tests with long vector arithmetic, the sphere's `TestVisibility` and line test, and
the CDB object and triangle tests also remain.
