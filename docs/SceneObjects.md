# Static, attachable and jaw scene objects

These classes are scoped views over the original objects: `include/game/SceneObjects.h`
declares only the members and methods the reconstructed code reaches. Bases, table
layouts and complete allocations stay unestablished. Frontline (CodeWarrior) supplies
control flow and meanings; every offset below comes from the Rising Sun disassembly, and
member names are descriptive unless a symbol names them. See
[SceneNode.md](SceneNode.md) for the node tables and [Targeting.md](Targeting.md) for AI.

## Provenance of the fragments

| Source | Original file evidence | Functions |
|---|---|---|
| `src/scene/StaticObjectAccess.cpp`, `StaticObjectMotionFlags.cpp`, `StaticObject*.cpp` | `static_obj.cpp` in the code map | see each manifest |
| `src/scene/AttachableObjectA.cpp`, `B`, `C` | `attachobject.cpp` in the code map | slot and child handles; `Draw`, `IsVisible`; the update forwards, `FindAttachNode`, `GetObjectType` |
| `src/ai/AIObject*.cpp` | `AIObject.cpp` in the code map | location, behavior, status and global-list helpers |
| `src/scene/jaw_controller_ctor.cpp`, `jaw_controller_motiontype.cpp` | **none** | constructor and `SetMotionType` |

The jaw controller sits in a code-map group with no file name (`.text @ 800d3588`). The
`jaw_controller_*` file names are borrowed from Frontline's reconstruction, so they are
not evidence of a Rising Sun source file, and those manifests record no `original_file`.
`CJawController`, its methods and `EJawType` are named by the mangled symbols; member
names are descriptive.

## AI object helpers (commit 8ea27c6)

`GetLocation` (both overloads) and the two `GetDistance*Real` functions, the behavior
selection group from `ChooseBehavior` to `GetBehaviorCapability`, the previous and next
behavior accessors, the `As*` casts through `SetAIStatusVar`, and the global-list add and
remove all match. `CBehavior` is a view added for them: the enable word is at `+8`, its
table pointer at `+0xc`; slot names come from the original table where it names them, and
the pure slots are descriptive only. `CAIObject` holds the previous, next and current
behaviors at `+0x370`, `+0x378` and `+0x37c`.

The AI object constructors and destructors are deliberately absent. Matching them would
need `CAIObject` modelled as a real base with its table pointer at `+0x72c`, and the
symbols do not establish that layout.

## Static object fields

Offsets are verified by the matching functions: motion fields from `+0x234` to `+0x290`,
the sort-list nodes at `+0x20`, four bounding-volume slots from `+0x130`, the flags word
at `+0x218` and the fade manager at `+0x2dc`. The older field names `worldVolume` (`+0x134`)
and `field_13c` are misleading and retained so that other units keep compiling:
`+0x134` is the type-0 *local* volume and `+0x13c` the type-0 *world* volume.

The `.rodata` of `static_obj.cpp` is not continuous across the matched functions. After
`SerializeSaveData`'s literal, eleven words of 1.0 and 0.0 precede the literal of
`GetWorldLinearVelocity`, and no function in `.text` references them. They look like the
pool of a linker-stripped function, which is an inference. The units split there
rather than reconstruct a dead function.

## Animated and hierarchy objects

`CAnimObject` and `CHierObject` follow the same per-offset rule. `CHierObject` derives
from the `CStaticObject` view: both `CStaticObject::Draw` and
`CAttachableObject::UpdateAttachedObjects` run on the same pointer, so those views start at
the first byte. Its members begin near `+0x980`; each entry of its bullet-damage table is
12 bytes. `CHierAnimObject` holds its animated object at `+0xb40`, and that object has a
second table pointer at `+0x38` whose `+0x10` slot returns the skeleton; the attach point
is named `JO_gun` in the data. `CTankObject` stores its AI doodad bytes at `+0xb40`, the AI
object at `+0xb4c`, slot mappings at `+0xb58` and three weapons at `+0xb78`. In
`objcreate.cpp` the box, sphere and animated volume allocators use the file's local pools;
each `BSGO_*` class places script data at `+0x14` and its node after it.

`CSoldierObject` stays a view unrelated to `CAnimObject` because existing members use other
names for the same offsets; its methods convert the pointer unadjusted. Eight 112-byte attach
slots start at `+0xba0`. `CWorldObject` returns one embedded volume at `+0xd0` for every type.

The scene-object casts in these units pass a pointer unadjusted. That follows the scoped
views and does not assert the original class layouts or inheritance order.

## Object creation

`CreateDumpObject` builds a stack `BSGO_Dummy` (`BSGO_Basic` plus a `WeakPtr<ISceneNode, 8>` and four
proximity words) so `ForceUpdateProximityTriggerStatus` can see the new object. `BSGO_Basic`'s destructor
is implicit: the weak copy has no table store and callers reduce it to `ISubject`'s. The string "Team"
at `0x802a705c` is first used by `CreateGerman` and shared by `CreateCollisionVolume`,
`CreateSpecialThrownObject`, `CreateRootHierObject`, `CreateDestructable` and `CreatePlayer`, so those
functions can only verify inside one unit with `CreateGerman`.
`objcreate.cpp` keeps its pools and queues in file statics, declared in `ObjCreate.h`. The dump
container holds a fixed array of 56-byte `DumpObject` entries linked oldest first. Its capacity comes
from a level resource, with 25 as the fallback, and `DeleteOldest` skips entries marked to keep.
`CreateObject`'s types are 1 mechanic, 2 environment-modifier mechanic, 3 enemy, 4 powerup, 7 to 9
particle systems, 10 projector and 11 animated mechanic. A delayed destruction runs once its entry is
more than one frame old. A queued German counts towards the non-visible soldier cull once it has
waited more than six frames. `CreateEnemy` compares the child script with `BOT3` to choose a bot.
`ShutdownObjectCreators` rebuilds the container in place, and the original tests the address first.
Each `BSGO_*` destruction unlinks the object, removes its scene node, destroys the script object and
deletes itself; each class keeps a 24-byte block with its scene node at `+0x14` after the script data.
`DestroySingleQueueElement` sends message 264 before calling `Destroy`.
`DeleteSpecialThrownObject` calls `CleanUp` at table offset 840, one slot before the attachable table's,
because a thrown object's table has no `FindAttachNode`.

`CStaticObject::SetMovementPath` keeps its path and look traversals at `+0xd0` and `+0xa0`. In
`MakeNoise`, the object skipped by the loop is read from a stack structure that is never written in this
build, so the comparison reads uninitialized memory.

## Parked

These still differ, with instruction differences in brackets; drafts are kept outside the
tree and a later reader should expect them to be close rather than solved.

- `CStaticObject::CalculateMovementTime` (27): register allocation of the three times.
- `CStaticObject::AnimUpdate`: one missing `fmr`, which shifts the rest; the period and its
  conversion share a register in this build.
- `CStaticObject::AttemptAnimMotionUpdate` (3): one scheduled load.
- `CStaticObject::StartMotionPlayback` (60): the key-frame temporaries.
- `CAnimObject::BeginUpdate`, `DetectGround` and `Yaw`: a local's address stays in a saved
  register where the original rematerializes it.
- `CHierObject::GetWorldLinearVelocity` (19): register choice.
- `CSoldierObject::GetBulletEmitterMatrix`: the original computes attach-slot addresses as
  `(this + i * 112) + offset`. `CleanUp` and `DetachObject` reproduce that with a pointer offset by
  the slot and `attachSlots[0]`, a source form that is inferred; this function still differs.
- `CAITankObject::SetInitialStatus` and `UpdateStatus`: a 15.0 literal sits at a 4-byte
  boundary before the double. A `static const float` in a `.rodata.*` section is folded back
  into the pool, so the subsection mapping cannot place it.
- `NullDeallocateArchiveBuffer`, `CompareObjectDefinitions` and `CompareStaticObjectDefinitions` have
  local binding and no caller in a matched unit; the two comparisons match when compiled alone.
- `CAIObject::ComputeStatusThreatValue` (18): the value and status pointer swap registers and
  the result is not kept in a saved register.
- `CAIObject::IsObjectInLineOfSight(const CVector3 &, float, bool, const CAITarget *)` (49):
  the only cause is that the target-vector address is hoisted out of the height loop. It holds
  the const overload back too: that function and `IsObjectOccluded` share the two merged
  height tables and the literals are contiguous, so it cannot land in a unit of its own.
- `CAIObject::IsObjectReachable` (44): a second temporary copy of the start vector.
- `CAIObject::FindRunAwayPoint` (25): the shared failure return sits before the search, where this
  build moves the zero result into a conditional store.

The script-motion bits in `CStaticObject` are tested through inline accessors that each return
one bit; without them `||` merges the tests into one mask. The accessor names are descriptive.
