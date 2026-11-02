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

These discarded inline constructors are inferred from layout (the orphan literal and the
order of the fiber and capsule files), not proven by any symbol.

`IVolume`'s virtuals are all inline: they sit in table order in the linker's weak-code
region at `0x8027982c`, away from any file's own methods. `IVolume.h` keeps its
`#pragma interface`, as the other volume headers do so that a fragment does not emit
them, and `volume_defaults.cpp` emits them with `#pragma implementation`. That pragma
pair and the file are project arrangements: the original's file for these is unknown.

## World volume

`CWorldVolume` holds its sub-volumes in a `dwi::list`, a fixed-pool intrusive list.
`RemoveSubVolume`'s mangled symbol names `dwi::list_iter`, and the string
`source/tl/dwi_memory.h:56` is the label of the pool allocation, so the node allocator
lived at that line of a header the original tree calls `dwi_memory.h`. The list's other
members, the helpers (`insert`, `erase`, `swap`, `Exchange`) and the `DwiList.h` header
name are inferred from the inlined code. `operator!=` takes its operand by value because
the original keeps a copy in the frame each time round the loop.

Two `1.0f` words at `0x802a83c4` and `0x802a83c8` sit between the constructor's literals
and `UpdateExtents`'s. No retained function reads them, so the world volume's text is
split at `UpdateExtents` to leave them out; what emitted them is unknown.

## CDB objects

`CCDBObject`'s box, sphere, capsule and animated tests gather tri groups into a
`dwi::fast_vec` over 2048 stack words, starting at the database's root branch or, with no
branches, its root leaf (`branchCount` at `+28`). The sphere's test inlines its own plane
test instead of calling `TestCollisionWithTriGroups<CVolSphere>`, which is why no such
instance exists. The leaf test for a box calls `CVolBox::TestCollision`, not the override
`CCDBVolBox` also has. Field names, `CDBTri`'s words and `CDBVector` are descriptive.

`_._10CCDBVolBox` sits in the weak-code region like `IVolume`'s virtuals; a small file with
`#pragma implementation` emits it with the class table, again as a project arrangement.

## Contacts and the test driver

`CCollision` is 36 bytes. Contacts come from `CCollision::ms_contactPool`, over the 300
48-byte records of `g_aClsnContacts`, and form a list through `+44`. `AddContact` adds
the word at `+20` to the separation; copying and merging set that word to zero first so
the stored values are not adjusted twice. `Test` pads both volumes' radii by
`LARGEST_MOVE_PER_TICK`, runs the test and restores them, and sets the function-local
`isCSFWarningEnabled.6234` after a checked test. The source names that local with an `__asm__` label, so the
compiler's own number is kept. `SClsnContactPool` and its inline
helpers are project names: no symbol records the pool's type.

## Not reconstructed

`CCollisionVolume::~CCollisionVolume` inlines the `ISceneNode` destructor, which
`SceneNode.h` declares out of line, and stays original context. The sphere and box
tests with long vector arithmetic, the sphere's `TestVisibility` and line test, and
the CDB object and triangle tests also remain.

## File attributions

`cdbobject_head`'s `original_file` and its index 533 were chosen from the file symbol's
name (`cdbobject.cpp`), not from the code map, which gives that address range no file.
`csg_volume` and `animated_volume_tests` carry the file symbols 1336 and 1325 in the same
way.
