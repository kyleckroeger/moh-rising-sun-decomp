# Sound

The ambient-track and settings fragments reconstruct eight functions (536
executable bytes) from the pinned GR8E69 executable, all from the original
`sound.cpp` (file record 1378). Claude Code
assisted the analysis and verification. Private `sound.cpp` storage and the private
`err` helper stay external original context.

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `sound_getters` | `SoundGetVolumes`, `SoundGetOneShotBankID`, `SoundGetAmbientTrackEvent`, `SoundGetMusicEventID` | 92 | None |
| `ambient_track` | `AmbientTrack_Start` (private), `AmbientTrack_Select`, `AmbientTrack_Volume` | 320 | 144 bytes at `0x802a9bc4` |
| `ambient_track_location` | `AmbientTrack_SetLocation` | 124 | 4 bytes at `0x802a9c90` |

## Behavior

- `SoundGetVolumes` copies `g_sfxVolume` and `g_musicVolume` unless the global
  `g_bDisableSound` is set. The other getters copy `g_OneShotBankID`, the ambient
  event's current track (`+0x48`) and the global `gMusicCurEvent`.
- One ambient stream is driven through the private 76-byte `gAmbientTrackEvent`
  (`AmbientStreamSoundEvent`) and its handle `g_hAmbientTrack`; -12 means no stream.
- `AmbientTrack_Start` clears the event's word at `+0x24`, sets the float at `+0x28`
  to 0 and both halfwords at `+0x3c`/`+0x3e` to 70, then calls `SendEvent`. A
  negative result prints the original error through `err` and `DebugMsg` and stores
  -12.
- `AmbientTrack_Select(track)` stores the requested track at `+0x44`; without a
  stream it starts one, otherwise it calls `UpdateEvent(handle, 1)` only when the
  track differs from the current one.
- `AmbientTrack_Volume` stores the volume at `+0x34` while a stream exists.
- `AmbientTrack_SetLocation(position)` sets `g_ambientSourceOnPlayer` when the
  position is null; otherwise it copies the position into the 16-byte
  `g_ambientSourcePos` (a `CVector3`).

`include/game/SoundAmbience.h` is a member-only view of the event: it names the
touched fields and the two methods, and declares no table pointer or base class.

The `.rodata` between the two fragments holds an `AmbientTrack_Stop(): EndEvent()
FAILED` message. No `AmbientTrack_Stop` code exists in the executable; it is left as
original context rather than given an invented body.

Three [script built-ins](Script.md#objective-ambient-sound-and-value-built-ins)
now call these accepted ambient-track helpers. They select a track, convert and
clamp an integer volume percentage, and choose the current script trigger's
position or player-relative ambience. These 536 code bytes and 24 literal bytes
are verified in `src/script/` and counted separately from the sound functions above.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
fragment and its strings and constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.

## Grenade and bazooka sound events

Following the [explosion and thrown-bullet investigation](research/explosion-grenade-updates.md),
six fragments reconstruct twelve functions and **556 executable bytes**. These
include the `SoundAliasEvent::SendEvent` dispatcher and
`PositionSoundEmitter::SetPosition` used by `CThrownBullet::CommitUpdate`.
Codex assisted reconstruction directly from GR8E69; no external implementation
is imported. The fragments generate no allocated data and use the same ProDG
3.8.1 profile above.

| Manifest | Functions | Address | Code bytes |
| --- | --- | --- | ---: |
| `sound_alias_send` | `SoundAliasEvent::SendEvent` | `0x801679b0` | 320 |
| `sound_alias_destructor` | `SoundAliasEvent` deleting destructor | `0x80167b30` | 52 |
| `position_sound_copy` | `PositionSoundEmitter::Clone`, `SetPosition` | `0x801680a4` | 76 |
| `position_sound_defaults` | deleting destructor, `GetObjRef`, `GetType` | `0x8016831c` | 68 |
| `sound_event_defaults` | `SoundEvent::UpdateEvent`, `EndEvent`, `SetEmitter`, `SetListener` | `0x80283618` | 32 |
| `sound_emitter_defaults` | `SoundEmitter::GetFilterDef` | `0x8028366c` | 8 |

`SendEvent` first clears the private `SoundAliasEvent.cpp` pool, including when
the alias is zero. For a nonzero alias it asks `g_SoundMan` to fill the pool.
An empty list returns zero. Otherwise it sets each entry's emitter; when the
event's word at `+8` is nonzero, it also copies that word, `+0x1c` and the three
words at `+0x10` to entry offsets `+0x84`, `+0x88` and `+0x90` respectively.
It then schedules the list and returns the schedule result. The words retain their offset names. The entry-lifecycle reconstruction below
identifies the first two as a callback address and event mask; the three copied
parameter words still have unknown higher-level meanings.
The listener and float at `+0x0c` are not read by this function.

The initial `SoundEntryPool` view exposed its first 32 bytes, a
`dwi::list<SoundEntry *>`. The lifecycle work below extends that view through
the free-entry link at `+0x12e0`. The original private `s_entries` at `0x803e5874`
is 4,840 bytes; its storage and initialization stay original context. Its
manifest dependency is explicitly scoped to original file record 1455.
`Clear`, `SoundEntry::SetEmitter` and `SoundSchedule::AddSounds` are now
reconstructed separately below. Alias lookup remains an external body. `GetEntries`, `GetSchedule` and `SetFields` are descriptive inline
reconstruction helpers, not recovered original names or source boundaries.
The iterator's postfix increment preserves the observed old-iterator copies;
the existing prefix operation is unchanged.

`SoundEvents.h` follows the original virtual-slot order and observed stack
storage: emitter base 4 bytes, positioned emitter 16, event base 36 and alias
event 40. The positioned emitter holds three floats at `+4`, `+8` and `+12`;
it is not a four-component `CVector3`. `SetPosition` copies those three values
and preserves all other storage. `Clone` does nothing for a null destination;
otherwise placement copy construction writes the derived table pointer and
three floats. It does not allocate a destination or check its capacity.

The positioned emitter returns null for `GetObjRef` and 1 for `GetType`.
The base filter getter and event update/end defaults return zero. The setters
write only their respective pointer at event `+0` or `+4`. Pointer pointee types
for `GetObjRef`/`GetFilterDef`, and the unused return type of alias lookup,
remain provisional ABI declarations. The complete deleting-destructor bodies
are verified, including restoration of the base table pointer and conditional
object deletion. The inline base-destructor declarations earn no separate
credit for the still-original standalone base destructors.

Default event/emitter constructors and distance/azimuth methods are declared
but not reconstructed here. In particular, this header does not accept the
unmatched constructor experiments inside `CommitUpdate`.

All allocated sections, function boundaries and external references are
compared before complete-image verification. Sixty-three isolated PowerPC
fixtures compare original and production-linked execution: null and self clone,
three-component copying, pointer setters, deletion flags, zero aliases, empty
and multi-entry lists, optional parameter copying and scheduler return values.
Pool population, entry attachment, scheduling and heap deletion are stubbed.
These checks are separate from the unmatched explosion drafts and are not
Dolphin or game-runtime tests.

## Entry lifecycle and spatial playback

A follow-up reconstructs **eight functions / 1,352 executable bytes** across six
fragments. Codex assisted reconstruction directly from GR8E69, using the same
ProDG 3.8.1 `-O2 -G0 -fno-exceptions -fno-implicit-templates` profile. These are
dependencies of the sound dispatcher above and other events, rather than new
matching bytes in the still-unmatched explosion helpers or `CommitUpdate`.

| Manifest | Functions | Address | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `sound_entry_attach` | `SetEmitter`, `DoNotifyCallback` | `0x8015b8f4` | 176 | None |
| `sound_entry_spatial` | `Calculate3D` | `0x8015ba60` | 272 | None |
| `sound_entry_update` | `UpDateSoundParams` | `0x8015bb70` | 212 | None |
| `sound_entry_volume` | `CalculateVolDistance` | `0x8015bc44` | 84 | 8 bytes at `0x802a9fe8` |
| `sound_pool_entries` | `SoundEntryPool::Clear`, `AddEntry` | `0x80168720` | 364 | None |
| `sound_schedule_add` | `SoundSchedule::AddSounds` | `0x8015a284` | 244 | None |

Original file records are `SoundEntry.cpp` (1409), `SoundEntryPool.cpp` (1462)
and `soundschedule.cpp` (1406). The project sources remain fragments of those
files. Every generated allocated section and complete function boundary is
compared; no partial matches, discarded functions or original context earn
credit. The two volume constants are the float representations of `1/30` and
zero; these data bytes earn no code credit.

### Emitter attachment and callback filtering

`SetEmitter` stores null at entry `+0x50` for a null argument. Otherwise it points
that field at inline storage `+0x5c`, then invokes the supplied emitter's virtual
`Clone` with that destination. It does not retain the supplied pointer, allocate
storage, or destroy an earlier emitter. The original scene-emitter clone writes through destination `+24`, so the
aligned buffer occupies 28 bytes through entry `+0x77`, immediately before
the event ID. This does not reconstruct the emitter subclass itself.

`DoNotifyCallback` calls the address in entry `+0x84` with `(this, eventCode)`
only when the address is nonzero and the code shares a bit with the mask at
`+0x88`. There is no callback-result handling. This independently clarifies the
corresponding event words copied by `SendEvent`: event `+8` is the callback
address and `+0x1c` its mask. The declarations retain raw words to preserve that
32-bit interface. `SoundCBEventCode` is a recovered enum type name; its historical
enumerator names and full set of values remain unknown.

### Spatial state and volume

`Calculate3D` saves the previous azimuth and distance, then returns unchanged
when the emitter pointer is null. With an emitter it first calls `GetObjRef`;
only a null result leads to `GetType`. If both results are zero, a looping entry
sets its `+0x4c` word to 1 and keeps the cached spatial values. Otherwise it
queries azimuth, then distance. A changed distance sets dirty bit `0x2`, and a
changed azimuth sets `0x4`, preserving existing bits. These comparisons also run
on the missing-emitter-object path: a cached NaN distance sets `0x2` even when
no new distance was fetched. No higher-level name for the `+0x4c` word is claimed.

The distance-volume calculation preserves the original operation grouping:

```text
slope = volumeLimit * (attenuation * float(1 / 30))
if not (distance <= 0): v = volumeLimit - slope * distance
else:                  v = volume
if not (v < volumeLimit): v = volumeLimit
return v
```

The fields are `volume` at `+0x1c`, `volumeLimit` at `+0x20`, `attenuation` at
`+0x24`, and cached distance at `+0x54`. These are descriptive names, not recovered
member names. The final comparison imposes only an upper bound; negative results
are retained. The negated comparisons intentionally preserve unordered behavior.
No fast-math, reassociation or extra lower clamp is introduced.

`UpDateSoundParams` handles dirty flags in this order:

- `0x4`: call `SND3dpos(handle, azimuth, 0)` and retain its result.
- `0x2` or `0x1`: store distance-adjusted volume when entry `+8` is nonzero,
  otherwise the `+0x1c` value, at `+0xc4`; pass it to virtual `SetNormSndVol`.
- `0x8`: pass the signed halfword at `+0x18` to virtual `SetSndPitch`.

It clears the entire dirty halfword at `+0xa0`, including unrecognized bits, and
returns the `SND3dpos` result or zero when that call was skipped. It reloads flags
between callbacks; this is not a single snapshot of the initial flags.

### Pool reuse and scheduling

`Clear` walks the active list, explicitly invokes each entry's virtual destructor
without deleting the slot, places the slot on the free-entry chain at pool
`+0x12e0`, then erases/recycles the list node. Clearing an empty list does nothing.
The source starts a small free-link object's lifetime in the destroyed entry's
storage using placement construction; `SoundFreeEntry` and `Recycle` are
reconstruction helper names. No complete historical pool template is claimed.

`AddEntry` pops a free slot, asks the supplied entry to clone into it, and appends
the result to the active list. Both the entry slot and a list node must already
be available: the original supplies neither an exhaustion check nor allocation
fallback. The pool view is now a 4,836-byte prefix through that free-entry link;
it still does not reconstruct the full 4,840-byte object or its constructor.

`AddSounds` immediately returns zero when sound is disabled. Otherwise, a nonempty
list consumes one event ID from schedule `+0x228`, with 32-bit wraparound. For
each entry it applies the buss manager at `+0x22c`, assigns the event ID at entry
`+0x78`, conditionally calculates 3D state using entry `+8`, then calls `Add`.
A negative `Add` result returns zero immediately. Earlier entries and the consumed
ID are not rolled back, and later entries are untouched. A successfully processed
list returns its ID; an empty list returns zero without incrementing the counter.

The schedule remains a prefix view, and `cSoundBussMan` is method-only. Expanding
the schedule prefix preserves its position at sound manager `+0x837c` and the
sound-zone field at `+0x85f4`. Neither a prefix size nor a call establishes the
complete allocation of those objects. Sound-entry virtual slots are confirmed
by both original bank and stream tables, including signed `this` adjustments.
Unused external return types remain provisional where the accepted callers do
not consume them.

### Verification and remaining work

Six production-header builds verify all eight complete functions and generated
constants. Local original-versus-production-linked PowerPC fixtures exercise
**636 cases**: 6 emitter attachments, 60 callback masks, 80 spatial states, 220
volume cases, 144 parameter updates, 9 clears, 9 additions and 108 scheduling
cases. Checks include adjusted virtual calls, NaNs/infinities, signed pitch,
unknown dirty bits, ID wraparound, partial scheduling failure, free-list order
and preservation of selected untouched storage.

External callback, audio, clone, destructor, buss and scheduler operations are
stubbed where appropriate. These isolated checks do not establish exhaustive
equivalence or integrated Dolphin/game behavior. Complete-image verification,
the repository validation suite and the captured progress snapshot are separate
checks; existing matched function records must remain unchanged.

`SoundEntry::GetOpts` remains research: its current 188-byte draft differs in
register selection and earns no credit. The pool constructor, scheduler's `Add`,
full buss processing and alias lookup also remain external work. This follow-up
does not promote the explosion/update drafts documented in
[the previous research](research/explosion-grenade-updates.md).

## EA audio and REAL runtime libraries

`src/sound/ea/` and `src/ea/` hold EA's C-style audio (`SND*`, `SFILTER_*`) and
runtime (`FILESYS_*`, `MEM_*`, `MUTEX_*`, `SIGNAL_*`, `FONT_*`) units, one per file
group in the code map. The Frontline reconstruction is their semantic reference;
every function was compiled with ProDG 3.8.1 and checked against GR8E69. Their
record layouts and most member names are inferred; a type named in a mangled symbol
is evidence for the name only.

`sndgs` is 0x324 bytes here and its members sit at different offsets from
Frontline's: the bank slot table is at `+0x1f4`, the memory pool at `+0x220` and the
abort-message hook at `+0xc8`. The sound system's public calls are partly C++ in this
build (`Snd::System::Restore`, `Snd::CODASetNew`), where Frontline uses C names.

These libraries were built with `-fno-strength-reduce`: a kept `mulli` or indexed `lfsx`
in a loop is the absence of strength reduction, not a code-shape puzzle. The `-G` value
differs by library; see the note on small data below.

Other layouts that differ from Frontline: a voice record is 140 bytes rather than 128,
and the voice table sits at `sndgs+0x1f0` with the voice count at `sndgs+0x17e`.
`sndmix` holds a pointer to its 96-byte channel records at `+0x1d8`. The stream record's
members above about `+0xd0` sit 40 bytes further along than Frontline's.

`hlafile`'s `numrequests`, `freequeue` and `requestidcounter` are small statics of that
file. The original symbol table gives `.sbss` statics a size of 0, so the unit declares
them `extern` as original context instead of defining them. Their ownership is
therefore not verified by the build.

`include/game/SoundGlobals.h` and `SoundMixerView.h` are inferred member views of
`sndgs` and `sndmix`. Both are single struct objects in the original: their members are
reached through one base register, which a character-array view does not reproduce.

Small data is not uniform across these libraries, and units follow it. The REAL runtime
(`FONT_*`, `FILESYS_*`, `MEM_*`, `TIMER_*`, `THREAD_*`, `PRINT_*`) reaches its small
statics through `r13`, so its units use `-G8 -fno-strength-reduce`. The `SND*` code from
`0x80218efc` to `0x8022bed0` has 0 `r13` references across all 19,445 of its instructions,
and its 4-byte globals such as `Snd::gMutexLockFn`, `MIXstopfn` and `everyother` sit
outside `.sdata` and are reached with `lis`, so new units there use `-G0
-fno-strength-reduce`. That is an inference from the instruction stream, not a recovered
build setting. `SND*` units landed before this was established keep `-G8`; they match as landed and
have not been migrated.

Ownership of some small statics is not verified by the build: `g_thMain`,
`MEM_allocfailcallback` and the timer statics are declared `extern` as original context,
like `hlafile`'s statics above, rather than defined in a unit.
