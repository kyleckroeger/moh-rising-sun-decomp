# Collision volumes

`src/geometry/` reconstructs the double-dispatch layer of the collision volumes from
the pinned GR8E69 executable. Claude Code assisted the reconstruction. The user's
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
