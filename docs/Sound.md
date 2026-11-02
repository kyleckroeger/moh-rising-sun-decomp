# Sound: ambient track and settings getters

`src/sound/` reconstructs eight functions (536 executable bytes) from the pinned
GR8E69 executable, all from the original `sound.cpp` (file record 1378). Claude Code
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

These libraries match with `-O2 -G8 -fno-strength-reduce`: a `lwz rX,N(r13)` is small
data, and a kept `mulli` or indexed `lfsx` in a loop is the absence of strength
reduction, not a code-shape puzzle. New EA units use those flags. Earlier units keep
the flags they landed with for now.

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

Small data is not uniform across these libraries. The REAL runtime (`FONT_*`,
`FILESYS_*`, `MEM_*`, `TIMER_*`, `THREAD_*`, `PRINT_*`) uses `r13`; the `SND*` code from
`0x80218efc` to `0x8022bed0` has no `r13` reference, and its 4-byte globals such as
`Snd::gMutexLockFn`, `MIXstopfn` and `everyother` sit outside `.sdata` and are reached
with `lis`. `SNDSYS_entercritical` and `SNDSYS_leavecritical` match with `-G0
-fno-strength-reduce`. Units here are built with `-G8` for now, so code touching such
globals is not yet reconstructed.
