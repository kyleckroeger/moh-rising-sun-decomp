# PR #59 review and integration

[PR #59](https://github.com/lifewillbeokay/moh-rising-sun/pull/59), contributed by
kyleckroeger with disclosed Claude Code assistance, reconstructs 706 additional
functions covering 148,180 executable bytes. Codex assisted the maintainer review
and integration with `main` through PR #60. Frontline remains the attributed
semantic reference; the pinned GR8E69 executable controls byte verification.

## Integration corrections

All 19 accepted `hierobject.cpp` fragments now share `-ffast-math`, as do all ten
accepted `HeadsUpDisplay.cpp` fragments. The original proposal used the option
only for `SwapOut` and the compass fragment. The consistent profiles retain exact
output across both file groups. `DrawWeaponName` names its inner alpha and
vertical-offset products to preserve those calculation steps under the shared
profile. The toolchain remains ProDG 3.8.1; this establishes a working file-group
profile, not the historical compiler options.

The PR supplies exact versions of `CStaticObject::Init`,
`CStaticObject::AttemptAnimMotionUpdate` and `CTankObject::AddWeapon`. Their 860
bytes supersede three partials added in PR #60. The duplicate comparison entries
and partial bodies are removed; the existing differential fixtures now load the
accepted native units. The other nine functions in that batch remain partial.
Both branches' loader research is retained, and the snapshot is regenerated from
the integrated source rather than resolving generated counts by hand.

## Source review boundaries

The collision checker's 128-byte reserved extent and soldier attach-slot
alignment are documented inferences from code generation. They do not establish
complete historical class allocations and should not be propagated into new
allocation or serialization code without further evidence. The views' observed
member offsets remain distinct from those inferred extents.

The inline base destructors and implicit `BSGO_Basic` destructor model observed
inlining and weak bodies. Declared-only destructors remain external dependencies;
no replacement body is inferred merely to suppress output. The RCMP decoder's
unused emitted delete operator is explicitly inventoried as discarded, and earns
no credit. The normal verifier still checks all retained code, data and symbols.

The variadic wrappers retain their `.cpp` file evidence and copy the compiler's
`__builtin_saveregs` record using its declared `__va_list_tag`. This is a
compiler-specific source spelling of the argument-record copy, not a new ABI or
proof of the original macro. Changing language per function just to select a
different standard macro expansion is unnecessary here.

Uninitialized stack reads, the contiguous contact walk, repeated branch arms and
list-tail updates are preserved and documented as observed target behavior.
These reconstructions do not repair the original game. Exact native output is
strong evidence for this target, but is not a claim of portable C++ behavior or
recovery of every original type and statement.

## Follow-up leads, outside this merge

- Double-literal pools still require complete native section placement and data
  verification. Combine evidenced neighboring fragments or establish correct
  native subsection ownership; do not clip alignment bytes or constants.
- Shared strings need one explicit owner and complete surrounding storage. The
  existing combined-unit/shared-context patterns are starting points, not a
  waiver for overlapping accepted ranges.
- Unnamed private statics need evidence-based ownership support before use;
  inventing an original filename would hide the missing evidence.
- `_.tmp_0.7387` is outside the current BSS sequence-suffix normalization. A future
  extension should preserve binding, size, address and file-scope checks and add
  tests for guard-name collisions.
- Calls with no argument setup need caller/ABI analysis before changing a shared
  declaration. The parked thread/system restore cases stay excluded.
- An unused `bad_alloc::what` body needs provenance and complete native ownership
  before it is used to account for literal storage.

## Verification

The complete integrated build verifies all 2,020 accepted units and reproduces the
identical 2,860,576-byte analysis DOL. All 4,402 previously exact functions remain
covered; the 706 additions bring the total to 5,108 functions and 1,066,936 of
2,492,032 executable bytes (42.813896% exact and linked). Restored-library coverage
remains 392,216 bytes. The 22 retained comparison receipts are unchanged; weighted
similarity is 44.907878%.

The local suite passes all 92 tests using optional Unicorn 2.1.4, including the
1,023 shortlist scenarios and 750 explosion/grenade scenarios. External callees
are stubbed in those fixtures and floating traces round to six decimal places.
Snapshot capture and public export pass, and the report partitions the complete
executable without overlap. The code map agrees with the proposal's accepted
ranges. CI validates this locally verified snapshot; it does not rebuild the game.
No Dolphin/game runtime test was performed.
