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
It then schedules the list and returns the schedule result. The unknown words
retain offset names; their higher-level meanings have not been recovered.
The listener and float at `+0x0c` are not read by this function.

The scoped `SoundEntryPool` declaration exposes only its first 32 bytes, a
`dwi::list<SoundEntry *>`. The original private `s_entries` at `0x803e5874` is
4,840 bytes; the remainder and its initialization stay original context. Its
manifest dependency is explicitly scoped to original file record 1455.
`Clear`, alias lookup, `SoundEntry::SetEmitter` and scheduling remain external
bodies. `GetEntries`, `GetSchedule` and `SetFields` are descriptive inline
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
