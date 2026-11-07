# Explosion helpers and thrown-bullet update research

These notes describe the pinned GR8E69 executable, SHA-1
`dd797412be7d631e78c98f194a1bb47981174d75`. Codex assisted analysis and local
PowerPC execution checks. Thanks to kyleckroeger for
[PR #47](https://github.com/lifewillbeokay/moh-rising-sun/pull/47), which supplied
the bullet/scene reconstruction that made this follow-up possible. The previous
Frontline comparison remains attributed in [Bullets.md](../Bullets.md).

**The four functions below remain unmatched.** Their behavior has been checked
on selected inputs; that does not make their C++ drafts accepted matching source.
Experiments remain in ignored `scratch/`. No partial bytes, copied original
code or new constants from these drafts are counted in progress.

| Function | Original address | Original bytes | Current draft bytes | Remaining difference |
| --- | --- | ---: | ---: | --- |
| `CExplosion::ShouldHeightCheckBeFlipped` | `0x80154e54` | 216 | 216 | The true-result load is scheduled before the vector copies instead of after the comparison. |
| `CExplosion::TestRay` | `0x80154770` | 588 | 588 | Matrix arithmetic, temporary lifetimes and register scheduling. |
| `CExplosion::CheckForObstructions` | `0x801549bc` | 1,176 | 1,184 | Cross-product temporaries, saved registers and height/sign lifetimes. |
| `CThrownBullet::CommitUpdate` | `0x80152bf4` | 1,988 | 1,984 | Sound-constructor zero initialization, flag registers and the final matrix address lifetime. |

These drafts use the established ProDG 3.8.1
`-O2 -G0 -fno-exceptions -fno-implicit-templates` profile. Their complete
read-only sections match: 8 bytes at `0x802a9840`, 4 at `0x802a9814`, 40 at
`0x802a9818` and 112 at `0x802a959c`, respectively. Matching constants earn
no code credit. Alternate return forms, vector accesses, constructor spellings
and compiler comparisons have not established an acceptable match; no compiler
profile changes or instruction patches were accepted.

## Height predicate

Only script bullet types 21, 22 and 23 query the node position. For those types,
the result is whether `sphere.center.z - node.position.z` is strictly greater
than 2. Other types return false without querying the position. Equality and an
unordered comparison return false. The vector getter/assignment temporaries
are relevant to matching even though only the final z difference is used.

## Ray helper

The helper transforms each supplied offset by the supplied matrix, including
the matrix translation. It then adds the explosion sphere center to the start
and the supplied node position to the end. Thus these are not simply two
direction-vector transforms.

It constructs a `CLine3` from those points, constructs a collision record,
clears the record's `lineTest` flag, obtains the world node's bounding volume
with selector 0, and calls its line-collision overload with the final boolean
argument false. It returns whether the collision result is **not 1**, then
destroys the collision record. Preserve this raw result test rather than
assuming every collision enum has conventional truth-value semantics.

The world object remains a member-only view. Its virtual bounding-volume query
uses the separately established `ISceneNode` interface, including the table's
signed `this` adjustment.

## Obstruction sampling

The direction starts as node position minus sphere center with z set to zero.
Its XY length is computed, and all three components are scaled by the reciprocal
length when the length is nonzero. A length strictly below `0.001` returns false
before any ray tests.

The matrix basis uses `right = direction cross (0, 0, 1)`, front = direction,
and `up = right cross direction`, with zero translation. Sample width is 0.25;
the initial sample height is 1.6 and its default scale is 1.

- For a player, crouching selects a scale of 0.33. The node position's z is
  reduced by the float at player `+0x18c` (currently `localVolume + 12`). That
  access alone does not establish a complete player bounding-volume type.
- Otherwise, for a soldier, the look-matrix position supplies the sample height.
  The look z minus node z is replaced with zero when it is not `>= 0`, then
  divided by 1.6 to obtain the scale. The negated comparison matters for NaNs.
- The height predicate selects an integer sign of -1 or +1; the original converts
  that integer to float. Only the start offset's height receives this sign.

After applying the scale to height `h`, the three offset pairs are:

| Sample | Start offset | End offset |
| --- | --- | --- |
| High | `(0, 0, h * sign)` | `(0.25, 0, h)` |
| Middle | `(0, 0, h * (sign * 0.5))` | `(-0.25, 0, h * 0.5)` |
| Low | `(0, 0, h * (sign * 0.01))` | `(0, 0, h * 0.01)` |

Each false `TestRay` result returns false immediately. All three must return
true for the obstruction helper to return true. Keep the multiplication grouping
above; reassociation can change floating-point results and generated code.

## `CThrownBullet::CommitUpdate`

The word at bullet `+0x25c` controls several paths. Bit numbers below count from
the least significant bit. Bit 24 returns immediately, including skipping the
attached-light update. Existing field names such as `noCatch` are scoped names
from earlier accessors, not a complete interpretation of this flag word.

For either supported type group, the movement path is selected when:

```text
not (field_b0 <= 0) and (bit28 is clear or bit27 is set)
```

`field_b0` is the float at `+0xb0`; bit 28 is the pending-explosion flag and bit
27 is the caught flag. An unordered timer comparison therefore allows the
movement test to continue. Both movement paths copy the matrix position into
the cached position and velocity into `spin`.

| Thrown-bullet types | Additional movement behavior | Detonation path |
| --- | --- | --- |
| 1, 2, 4, 5, 7, 9, 10, 11 | Except type 4, bit 29 becomes the inverse of old bit 31; all clear bits 31 and 30. | If caught, skip explosion/effect/sound/message; always mark for destruction. Otherwise use the grenade path below. |
| 0, 6, 8 | No flag changes here. | Use the bazooka path below, including when caught if the timer has expired. |

For the grenade path, the current matrix position is the origin. Except for
type 5, a stack `CExplosion` receives the scene, position, property blast radius,
strength 1, virtual damage, script bullet type and firing node, then false and
true for the final flags. This explosion is destroyed before effect/sound work.
Type 5 still runs that effect/sound work. The effect's boolean argument is old
bit 31. A positioned emitter and alias `Explo_Grenade` produce the sound event;
message 141 is sent to the bullet's script object with the bullet as payload.

For the bazooka path, velocity length is computed in all three dimensions. When
that length is not `<= 0.001`, the explosion position is backed off by
`velocity * (-0.05 / length)`. It constructs the same form of `CExplosion`,
builds the effect matrix and passes `field_b0 > 0` as the effect flag. The sound
alias is `Explo_Bazooka`; notification and destruction follow. This stack
explosion survives through notification/marking and is destroyed at the end
of that branch.

Unknown types print the original warning. After the switch, a present attached
light receives the bullet's local-to-world matrix; this also occurs for an
unknown type. The initial bit-24 return is the exception.

## Verification and accepted dependencies

Local differential checks execute both original PowerPC instructions and the
compiled drafts on **714 scenarios**: 48 height cases, 10 transformed-ray cases,
96 obstruction cases and 560 update cases. They cover type boundaries, equality
and unordered height/timer inputs, matrix scale/rotation/translation, collision
result values, early ray termination, player crouch and soldier look heights,
catch/explosion flags, the type-5 exception, unknown types and attached lights.
Additional assertions check selected expected results and untouched object bytes.

These are isolated function checks. Virtual queries, scene/effect construction,
matrix setters, square root, collision bodies, sound scheduling, script
notification and destruction are stubbed as needed. The obstruction tests stub
the ray/height helpers individually. This is neither exhaustive equivalence nor
an integrated Dolphin/game test. A draft extending past its original function
boundary is handled by the research harness; it is not overlaid into the
accepted image and receives no matching credit.

The accepted result of this investigation is the separate
[sound-event reconstruction](../Sound.md#grenade-and-bazooka-sound-events):
`SoundAliasEvent::SendEvent` and eleven supporting methods, 556 matching bytes.
Its complete generated sections and twelve function boundaries are verified,
and 63 additional original/production-linked scenarios exercise those functions.
The pool, sound manager and unknown entry fields remain scoped views. The
unresolved default sound constructors are deliberately not promoted with them.

For the next matching attempt, use the branch conditions above as constraints,
then resolve the vector return/temporary forms and sound initialization
lifetimes. Do not move the height predicate's one differing instruction by
hand or count a behaviorally similar draft as matching source.
