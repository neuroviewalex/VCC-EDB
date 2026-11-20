# VCC-EDB baseline history

## 2026-11-20 — Stage A playlist readback and MPI R3, promoted

The tested R3 source now defines the authoritative opt-in AutoSequence status interface used by EDB v15. The user confirmed a normal EDB boot, status readings of `237, 1, 4` (signature, protocol version, four playlist entries), and successful playlist-aware `CD BACKGROUND`.

### Extended register protocol

Write `$ED` to `$7F02` to activate the readback interface; write `0` to deactivate it. The base address is configurable and defaults to `$7F00`.

| Offset from base | Readback |
| --- | --- |
| +1 | Currently mounted playlist entry (1-based; 0 if none matches) |
| +2 | Playlist entry count |
| +3 | Selection status |
| +4 | Last requested selector |
| +5 | Signature `$ED` |
| +6 | Protocol version `1` |
| +7 | Completion counter |

Legacy `POKE base,0` remains NEXT; nonzero selectors `1..count` select an exact entry. A completed selection is reported only after the corresponding image is mounted, and the disk-controller safe-swap mechanism is preserved.

### MPI corrections through R3

- **R1:** Forward activation writes through base+2 and status reads through base+7.
- **R2:** Export `PakCpuMemoryRead` from `MPI.DLL`, enabling the host read callback to reach the FD-502 module.
- **R3:** Track extended-readback activation independently for each MPI slot, bypass unactivated reads before taking the MPI lock or substituting cartridge bytes for RAM, and clear the gate on reset and cartridge changes.

These repairs keep normal pre-activation RAM reads intact when FD-502 is behind MPI. The original FD-502 write interface and unrelated emulator components were preserved.

### Verified Windows runtime

- `VCC-EDB-StageA-MPI-R3-Win32.zip` (when distributed with this repository).
- SHA-256 `92c7ca10e0de2091df38eb9c9ded2aaf55dfdfc51636c2a978980d71ad955e52`.
- Windows Release build, EDB ROM packaging, ZIP integrity, and `MPI.DLL`/`FD502.DLL` read exports passed automated checks. Actual playlist `CD` behavior was confirmed by the user in VCC.

The README was deliberately left unchanged.
