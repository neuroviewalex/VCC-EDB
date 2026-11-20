## **VCC-EDB versus official VCC 2.1.9.3**

The distinction is that **VCC-EDB is intended to be a portable, EDB-configured distribution of VCC**, rather than a fundamentally different CoCo 3 emulator.

**Portable EDB ROM selection.** VCC resolves `coco3_201_edb.rom` and `disk_211_edb.rom` relative to the executable's directory, rather than relying on paths from the machine where the package was built.

## **Complete documented feature overview**

### **1. Programmable disk-image playlist system**

- Supports up to **255 numbered DSK images**.
- A CoCo program can request a disk change by writing to address `&H7F00`.
- For example, `POKE &H7F00,5` selects disk image number 5.
- Repeated writes of the same value are recognized as separate commands.
- Disk changes are queued until the floppy controller becomes idle.
- Switching disks does not reset the emulated CoCo.
- An active disk operation is not intentionally interrupted.

This provides a mechanism for multi-disk software, automated demonstrations, and EDB programs to control disk changes from inside the emulated machine.

### **2. Floppy controller reliability improvements**

**FD-502 lifecycle and reset handling:** Corrects controller initialization and reset behavior, particularly when the emulator starts or resets.

**WD1793 transfer handling:** Improves reset handling when floppy transfers are in progress.

These changes are especially relevant to software performing continuous or automated disk operations.

### **3. Multi-Pak Interface and cartridge reliability**

VCC-EDB includes changes to:

- Refresh MPI cartridge-window handles after cartridge changes.
- Correct cartridge DLL initialization and cleanup.
- Improve hard-disk and SDC plugin lifecycle management.

The purpose is to make cartridge changes, resets, and plugin loading more reliable.

### **4. EDB ROM integration and automatic boot**

VCC-EDB distributes two customized ROMs:

| ROM                 | Function                                             |
| ------------------- | ---------------------------------------------------- |
| `coco3_201_edb.rom` | Extended Color BASIC 2.01 with EDB boot support      |
| `disk_211_edb.rom`  | Disk Extended Color BASIC 2.11 with EDB boot support |

The customized ROMs provide cleaner boot presentation and conditional startup behavior.

Without a disk controller, the CoCo ROM follows its normal Extended BASIC boot path. With a disk controller, it can hand off to the customized disk ROM.

The disk ROM supports conventional Disk BASIC startup and an EDB-oriented boot path when suitable boot media is present.

**Important distinction:** These are changes to the supplied ROM firmware, not necessarily new capabilities implemented in the VCC emulator core.

### **5. Portable configuration and ROM paths**

VCC-EDB now uses the directory containing `Vcc.exe` as the reference point for its portable configuration.

This includes:

- Application-local `Vcc.ini`.
- Automatic selection of the two EDB ROMs.
- ROM paths reconstructed from the executable directory at startup.
- Reduced dependence on absolute paths from a previous Windows installation.

The release directory can therefore be moved without manually correcting the ROM paths.

### **6. Custom keyboard layout portability**

VCC-EDB adds startup handling for its custom keyboard mapping.

- New configurations default to the Custom layout.
- `custom.keymap` is loaded from the executable directory.
- Application-local keymap paths can be stored relatively.
- If the custom keymap cannot be loaded, VCC falls back to a built-in layout for that session.

This is particularly useful when distributing a preconfigured EDB environment across computers.

### **7. Startup initialization improvements**

The initial hard reset is deliberately performed after the MPI and cartridge components have been loaded.

This matters because the CoCo ROM and disk-controller ROM must be active before the first emulated instruction executes.

The change addresses startup-order problems that could otherwise leave the emulator using the wrong ROM or an incompletely initialized disk controller.

## **What remains standard VCC functionality?**

The repository also documents the following emulator capabilities, but these should **not** be advertised as exclusive VCC-EDB inventions:

| Capability                             | Classification |
| -------------------------------------- | -------------- |
| Motorola 6809 / Hitachi 6309 emulation | Existing VCC   |
| CoCo 3 memory expansion up to 8 MB     | Existing VCC   |
| Four-slot MPI                          | Existing VCC   |
| FD-502 and four virtual drives         | Existing VCC   |
| Becker Port / DriveWire connectivity   | Existing VCC   |
| Virtual hard disks and SuperIDE        | Existing VCC   |
| Orchestra90cc                          | Existing VCC   |
| CoCo SDC simulation                    | Existing VCC   |

## **Overall assessment**

The most distinctive VCC-EDB capabilities are **software-controlled disk-image switching, EDB-aware ROM booting, improved controller initialization, and a portable, preconfigured environment**.

## VCC - Virtual Color Computer
VCC is an emulator for the Tandy Color Computer 3, designed to run on Microsoft Windows. It aims to provide an accurate and easy-to-use emulation of the original hardware, as purchased from a Radio Shack store or Tandy Computer Center between 1986-1992.  For VCC usage, see the User Guide at [VCC Wiki](https://github.com/VCCE/VCC/wiki). The wiki also contains release notes and additional documents. Online documentation may reflect some features not available in earlier versions or that are pre-release.

VCC is licensed under the GNU General Public License v3.0. See the [LICENSE](COPYING) file for more details.

## VCC-EDB Features the following patches:

    FD502 reset handling: corrected reset and lifecycle behavior.
    WD1793 floppy controller: reset handling for in-flight disk transfers.
    MPI cartridge window: refresh of window handles after cartridge changes.
    Cartridge DLL lifecycle: initialization and cleanup fixes.
    Plugin lifecycle: hard-disk and SDC plugin initialization/cleanup.

With a new extended storage solution:
Programmable disk-image playlist system...

    Up to 255 numbered DSK images
    Select disk using CD (by volume name) or SEQ (by index #)

Additional safeguards:

    Repeated identical CPU writes each detected as separate commands.
    Disk swaps are queued until the floppy controller is idle.
    Disk switching does not reset the CoCo.
    Never interrupts an active disk operation.

## DOWNLOAD THE CUSTOM COCO DECB ROMS!!

CoCo3 EXTENDED COLOR BASIC v2.01 and DISK EXTENDED COLOR BASIC v2.11
These ROMs can be burned and installed in your CoCo3, or used in emulators like VCC.
Changes for CoCo3 ROM:

    aesthetic improvements for boot up, no FoC (flash of color) corruption on both paths
    path 1 is standard ECB 2.01 boot up (normal), this is when there is NO DISK CONTROLLER PRESENT. This will exit normally to ECB but it will say v2.01 so you know it has EDB auto-boot capabilities.
    path 2 is all black, overclocked boot up, this is when there IS A DISK CONTROLLER PRESENT. hands it off to the disk controller for continued boot...

Changes to DISK ROM:

    aesthetic improvements for boot up, no FoC corruption on both paths.
    path 1 is standard DECB 2.11 boot up (normal), this is when there is NO EDB BOOT DISK in drive 0 (NOR any disk with *.BAS on it)
    path 2 is continued ALL BLACK OVERCLOCKED EBD boot, this is when there IS THE EDB BOOT DISK in drive 0 (OR any disk with *.BAS on it)

You can download these and select them in VCC under CPU, and under FD502 disk controller.

DOWNLOAD HERE! https://www.facebook.com/share/p/1MbmLtcLpH/

## Portable EDB first-run files

For the EDB portable profile, keep these companion files beside `Vcc.exe`:

- `coco3_201_edb.rom` — external CoCo 3 BASIC ROM.
- `disk_211_edb.rom` — external FD-502 disk-controller ROM.
- `custom.keymap` — portable custom keyboard map.

A new configuration defaults the keyboard layout to **Custom** and looks for
`custom.keymap` in the application directory. The initial hard reset is
performed after the MPI and its FD-502 slot are loaded, so the configured CoCo
and disk-controller ROMs are active before the first emulated instruction.
App-local keymap paths are stored relative to the executable directory so the
portable folder can be moved as a unit. If `custom.keymap` is absent or cannot
be read, VCC falls back to its built-in CoCo keyboard layout for that session.

## Features
VCC emulates a stock 128k CoCo 3 and additional products, including:
- **Tandy MultiPak Interface (MPI)**: Four expansion slots.
- **Tandy FD-502 Disk Drive Controller with Becker Port**: Includes Disk Extended BASIC,four configurable virtual disk drives, and a Becker Interface for connecting to a DriveWire Server
- **Virtual Hard Drive Interface**: Allows VCC to connect two Virtual Hard Disks (VHDs).
- **SuperIDE Hard Drive Controller**: Emulates dual IDE hard drives.
- **Orchestra90cc**: A five-voice music sequencer with stereo 8-bit DACs.
- **Becker Port**: Old (previous to V2.1.9.3) VCC Interface for connecting to a DriveWire server.
- **Memory Expansions**: Up to 8192k.
- **CPU Replacement**: Swap the Motorola 6809 CPU with a Hitachi 6309.
- **SDC Simulator**: Simulates a COCO SDC floppy emulator.

## Obtaining VCC
Sources and binaries for VCC versions since v2.0.1 can be found at [VCC Releases](https://github.com/VCCE/VCC/releases). It is recommended to use the "latest" release.

VCC version numbering is somewhat erratic. Currently, the version number consists of "Vcc-" followed by four numbers separated by dots, for example: "Vcc-2.1.8.3". The first number represents a fork; the current fork is "2". The second number represents a major version, the third number represents releases that make additions or significant changes, and the fourth number represents bug fixes or minor changes.

VCC binaries provided before v2.1.9.3 are not digitally signed and newer versions are privately signed by a VCC team developer.  It is likely you will be presented with Windows security warnings when you first run them.  Occasionally Virus detection software might flag VCC binaries due to false positives, even if you build from sources.  Every effort is taken to keep VCC safe to use but there is no warranty that it is.

## Building VCC
VCC is written in C++ and Microsoft Visual Studio 2022 Community is used to build VCC.  VS2022 is available for free download from Microsoft. The standard VCC build will run on Windows 10 and above. Optionally Visual Studio 2022 can be used to build a "legacy" VCC version using xp build tools that will run on Windows XP.  It is planned that VCC version 2.1.9.3 will be the last version to support XP.

To build VCC from the command line, launch the "Developer Command Prompt for VS 2022". From there, change to the directory containing the VCC sources and type "Build" or "BuildClean".

Within Visual Studio, the "Release" and "Debug" configurations build VCC binaries that will run on current Windows versions. The "Legacy" configuration builds binaries that will run on Windows XP. "Legacy" uses the v141_xp build tools from which you need to find and install.

Portions of VCC code have been modified to use features of the C++17 standard.

## Contributing to VCC
We welcome patches and code contributions that are consistent with our goals. Please comment your code well and add your name if you want credit for your work.

### How to Contribute
1. Fork the repository to your own GitHub account.
2. Clone the forked repository to your development system.
3. Create a new branch with a descriptive name.
4. Make your changes and commit them with clear messages.
5. Push your changes to your fork.
6. Submit a pull request to the `main` branch of the original repository.

Note: Do not include changes to `.sln` or `.vcxproj` files in your pull request. If you feel these project files need changes, contact the maintainers first.

We also welcome bug reports and suggestions. Please post these on the GitHub "Issues" tab. We check the issues periodically to see if there are fixes we can implement while working on VCC. The VCC Development Team is small, and we work on this project when we can, as we all have lives and families. VCC is a hobby, not a priority. It is a work of love, as we also use this software ourselves, so we try to make it as usable as possible. Sometimes progress is slow, and it looks like nothing is happening (and it may not be), but usually, there's plenty going on behind the scenes, and we have not committed our current work.

