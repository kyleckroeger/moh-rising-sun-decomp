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

## Parked

`Init` (2 diffs: the `+0x214` and `+0x110` stores swap), `CalculateMovementTime` (27:
register allocation of the three times) and `AttemptUpdate` (29: the three script-flag
tests stay separate in the original, with a register copy of the flags; merged or
cross-jumped forms do not reproduce it).
