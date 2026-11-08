# Scene-node virtual tables

This note records the virtual-slot order of `ISceneNode`, `IMovingSceneNode` and
`IParticleSystem`, recovered from original virtual tables in the pinned GR8E69
executable. Claude Code assisted the analysis and verification.

## Evidence

These scene-node tables have no vtable symbols, but constructors store their
addresses and each entry's function pointer resolves to a named original function.
Entries are eight bytes: a signed 16-bit `this` adjustment at offset 0, two unused
bytes, then the function pointer at offset 4.
Calls load the adjustment with `lha` and the target with `lwz` at the same slot.
Slot 0 is zero in every table examined.

| Table | Address | Slots | Evidence |
| --- | --- | ---: | --- |
| `IObserver` | `0x802e9aa0` | 1-4 | Already documented in [Observers.md](Observers.md) |
| `ISceneNode` | `0x802e1680` | 1-67 | Slot 2 is `_._10ISceneNode`; the entry after slot 67 is zero |
| `IParticleSystem` | `0x802e2048` | 1-107 | Stored by `__15IParticleSystem`; slot 2 is `_._15IParticleSystem` |
| `CParticleSystem` | `0x802e18c8` | 1-108 | Stored by `__15CParticleSystem`; the entry after slot 108 is zero |
| `CCamera` | `0x802e8410` | 1-88 | Stored by `__7CCameraRC7CCamera`; independently checked for the [camera reconstruction](Camera.md) |

Slots 1-4 match the existing `IDestructible`, `ISubject` and `IObserver`
declarations. `ISceneNode` introduces slots 5-67. `IMovingSceneNode` has no
out-of-line destructor or emitted table, but `IParticleSystem` and at least twelve
other derived tables, including `CStaticObject`, `CHierObject`, `CWeapon` and
`CBullet`, place `Reset__16IMovingSceneNode` at slot 68. Slots 68-88 resolve to
`IMovingSceneNode` symbols or their overrides, and `IParticleSystem` begins at slot
89. `CParticleSystem` adds only `UseFade` at slot 108.

The inlined base constructors in `__15IParticleSystem` write the observer fields
at `+4` through `+20`, including the observer's owner pointer to itself at `+16`.
This agrees with the 24-byte `IObserver` prefix. They also zero words at `+24`
through `+52`; which base class owns those words is not established, so no
scene-node data members are declared.

## Declarations

`include/game/SceneNode.h` declares `ISceneNode` and `IMovingSceneNode`;
`include/game/ParticleRecipe.h` declares `IParticleSystem`. Both use
`#pragma interface`, like the observer headers, so no accepted object emits a
table. The headers were generated from the table entries. Method names, parameter
types and constness come from the mangled symbols; enum parameter types
`EClsnId` and `ISceneNode::EVolumeType` have placeholder members only.

Return types are not encoded in these symbols. They are chosen as follows:

| Return type | Evidence |
| --- | --- |
| `As*` casts return the named class (`CStaticObject *`, `CBullet *`, ...) | Each named class has its own override that returns `this` unchanged; `AsAnimatedPlayerObject`'s override belongs to `CAnimatedPlayer` |
| `GetAIObject` returns `CAIObject *`; `GetAIDoodad` returns `CAIDoodad *` | `CPlayerObject`'s overrides return a pointer field and the address of an embedded member; both classes have original symbols |
| `GetAttachedLight` returns `CLight *` | `SetAttachedLight` takes `CLight *`; `CStaticObject`'s override returns a pointer field |
| `GetLightVolume` returns `BPDLightVolume *` | `Enter`/`ExitLightVolume` take that type; `CAnimObject`'s override returns `CLightVolumeManager::GetVolume()` |
| `GetName` returns `const char *` | `CBotObject`'s override returns the address of a byte buffer at an odd offset; constness is not established |
| `GetCollisionId` returns `EClsnId` | `SetCollisionId` takes it; the default returns -1 |
| `IsDrawEnabled` and the `IParticleSystem` flag getters return `unsigned int` | Accepted overrides' normalized ABI results |
| `int` for `Constrain`, `Attach`, `Detach`, both bounding-volume getters, `IsVisible`, `GetScriptObject`, `GetTeam`, `GetPlayerIndex` | ABI placeholders: the defaults return constants, and the overrides examined so far do not establish a type (`GetTeam` and `GetPlayerIndex` read bytes; `GetScriptObject` reads a pointer) |
| `void` | Every other slot |

Callers that use a placeholder result need evidence before its declaration is
changed.

`CScene` is a member-only prefix view of the external `g_scene` object. Its named
fields include the world pointer and the list heads established by the visitor
routines below; the view is not a complete allocation declaration.

## Scene visitors

`scene_visitors` reconstructs three adjacent functions from the original
`scene.cpp` (file record 1338), using ProDG 3.8.1 with the existing
`-O2 -G0 -fno-exceptions -fno-implicit-templates` profile. Codex assisted this work.

| Function | Address | Code bytes | Traversal |
| --- | --- | ---: | --- |
| `VisitAllStaticObjects` | `0x80147a1c` | 112 | Head at scene `+0x24`, next link at node `+0x48` |
| `VisitAllLights` | `0x80147a8c` | 112 | Head at scene `+0x34`, next link at node `+0x3c` |
| `VisitAllNodesInVolume` | `0x80147afc` | 372 | Head at scene `+0x00`, next link at node `+0x1c` |

All three stop when `visitor.Visit` returns true and return true themselves;
exhausting a list returns false. `VisitAllNodesInVolume` can skip world objects,
then tests each node's requested world volume against the supplied volume. A node
without a world volume is tested using its position instead. Collision state 1
means no overlap; every other state reaches the visitor. The temporary collision
is destroyed on both early success and normal iteration.

The visitor result fixes the earlier `void` declaration and permits the
[explosion constructor](Bullets.md#explosions-and-projectile-updates) to match.
The next-link structures in `SceneVisitors.cpp` are deliberately member-only views:
the observed offsets do not establish which historical base class owned each
link or the complete size of any node. The fragment also verifies the four-byte
vector constant at `0x802a8944`, which earns no code credit.

Thirteen isolated PowerPC fixtures executed the production-linked constructor
and visitor fragments: empty/exhausted lists, early exits, adjusted virtual calls,
world skipping, volume and position tests (including collision states 0, 1 and 2),
collision cleanup, constructor fields and noise/visit/world callback order.
External queries, collision lifetime bodies and effect callbacks were stubbed.
These checks are not game or Dolphin runtime tests.

## Verification

ProDG 3.8.1 compiles virtual calls through the declared order at the original
offsets: `Stop` calls slot 91 (`+0x2d8`), `SetLocalToWorld` slot 71 (`+0x238`),
`Terminate` slot 1 and `Destroy`'s deleting destructor slot 2. Any missing or
misplaced declaration before those slots would shift those offsets. All particle
fragments accepted before the hierarchy was introduced still produce identical
objects, and the complete rebuilt analysis image is identical.

## Accepted defaults

`src/scene/` reconstructs 81 base-class functions (632 bytes) in seven fragments:

| Manifest | Range | Functions | Bytes |
| --- | --- | ---: | ---: |
| `scene_node_destructor` | `0x8027913c` | 1 | 48 |
| `scene_node_update_defaults` | `0x8027916c`-`0x802791e0` | 15 | 116 |
| `scene_node_centroid` | `0x80279220` | 1 | 52 |
| `scene_node_query_defaults` | `0x802792b4`-`0x802793ec` | 42 | 312 |
| `moving_node_light_defaults` | `0x802797fc`-`0x8027982c` | 8 | 48 |
| `moving_node_transform_defaults` | `0x80279950`-`0x80279984` | 13 | 52 |
| `moving_node_reset` | `0x8027ab08` | 1 | 4 |

Most defaults are empty or return a constant: `Constrain` returns 1,
`GetCollisionId` returns -1, and the casts and other getters return null.
`IMovingSceneNode::AsMovingNode` returns `this`. `GetCentroid` forwards to virtual
`GetPosition` (slot 21). `GetTMLocalToWorld` calls the accepted
`CMatrix::GetSlot(0)` on its output. The destructor stores `_vt.10ISceneNode` and
calls `IObserver`'s destructor; the table stays external original data.

## Vector defaults

With [the `CVector3` declaration](Matrix.md#cvector3-layout), two more fragments
match: `scene_node_vector_defaults` (`GetWorldLinearVelocity` returns a zero
vector by value; `GetPosition` zeroes three components) and
`scene_node_axis_defaults` (`GetRightward`, `GetForward` and `GetUpward` set the
unit X, Y and Z axes). `GetWorldLinearVelocity` now returns `CVector3`.

## Scene node lists

`CScene` keeps a node list (`+0x00`, `+0x04`, count `+0x44`) and typed lists for
static objects (`+0x24`), animated objects (`+0x2c`), lights (`+0x34`) and
projectors (`+0x3c`), with counts at `+0x48`-`+0x54`. Node links are at `+0x18` and
`+0x1c`; static and animated objects link at `+0x44` and `+0x48`; lights and
projectors at `+0x38` and `+0x3c`. List and member names are descriptive. The
insertion path shared by `AddPlayer_now`, `SetWorld_now` and `AddNode_now` updates
the list tail even when it inserts after another node. `AddNode_now` and
`RemoveNode_now` skip the extent lists for lights. Collision exclusions are at
`+0x140`, in a small array container with its count at `+0x148`.

## Next work

The slot map applies to every class derived from `IMovingSceneNode`; each derived
table should be checked against it rather than assumed.

## CScene sweep containers

`CScene` keeps two sweep lists at `+0xa8` and `+0xd4` (`dwi::list<ISceneNode::SExtent>`, named by
`SweepExtents`' symbol) and two node-pair vectors at `+0x100` and `+0x118`
(`dwi::fast_vec<CScene::UNodePair>`). `AddNodeExtents` enters a begin and an end extent for a node on
each list, keeping four links at node `+0x20`; `RemoveNodeExtents` finds each by node and begin flag
and returns the list node to the free list. `Reset` removes every node and empties all containers,
destroying the collisions in the vector at `+0x130`. The vectors at `+0x60`, `+0x78` and `+0x90` are
only emptied in the code reconstructed so far, so their element types are unknown. The collision
iterators' user copy constructors and clearing destructors, and the field names, are inferred from the
code shape. `DwiContainers.h` holds the containers without placement new, so that `SceneNode.h` can use
them in units that also include the `new` header.
