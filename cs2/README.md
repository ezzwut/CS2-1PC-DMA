# CS2 DMA ESP (1-PC & 2-PC Setup)

A high-performance, purely visual DMA cheat for Counter-Strike 2, optimized for both 1-PC (KVM/QEMU/VFIO or PCIe Loopback) and standard 2-PC DMA setups.

<div align="center">
  <h3>Powered by <a href="https://github.com/ezzwut">github.com/ezzwut</a></h3>
</div>

## Features
This software is strictly an ESP (Extra Sensory Perception) tool. All memory-writing features (Aimbots, Triggerbots, etc.) have been completely removed to keep the codebase clean, undetectable, and fully read-only.
- **Dynamic Box ESP**: Scales accurately with player distance.
- **Bone/Skeleton ESP**: Smooth bone rendering.
- **Head/Neck ESP**: Specific highlighting for the head hitbox.
- **Armor & Health Bars**: Dynamic color-coded bars indicating enemy status.
- **Weapon Names**: Shows the currently equipped weapon.
- **Team Check**: Automatically ignores teammates (Can be toggled off for Deathmatch).
- **Fully Customizable UI**: Sleek, modern ImGui menu with light/dark mode support.

## Setup Instructions

### Prerequisites
1. A compatible DMA card (Raptor, Enigma, Screamer, etc.) flashed with custom firmware.
2. [MemProcFS](https://github.com/ufrisk/MemProcFS) downloaded and installed.
3. [a2x/cs2-dumper](https://github.com/a2x/cs2-dumper) to fetch the latest `offsets.json` and `client_dll.json`.

### 1-PC DMA Setup (KVM/QEMU Virtual Machine)
If you are using a 1-PC setup, you must run Counter-Strike 2 inside a KVM/QEMU virtual machine with PCIe passthrough.
1. Install your DMA card in your host motherboard.
2. Pass your GPU through to your Windows Guest VM.
3. Run the cheat executable on your **Host Linux/Windows OS**.
4. The cheat will read the VM's memory without running any software inside the game environment itself.

### Standard 2-PC DMA Setup
1. Plug the DMA PCIe card into your **Gaming PC**.
2. Connect the DMA card via USB to your **Radar/Cheat PC**.
3. Run the cheat on the Radar PC.

### Installation & Usage
1. Place the compiled `cs2.exe` into your MemProcFS directory.
2. Download the latest `offsets.json` and `client_dll.json` from the cs2-dumper repository and place them in the same folder as `cs2.exe`.
3. Launch `cs2.exe`.
4. Press `INSERT` to open the ESP configuration menu.

## Build Instructions
1. Open `Dll1.vcxproj` in Visual Studio 2022.
2. Set the build configuration to `Release` and `x64`.
3. Build the project. The compiled executable will be placed in the `x64/Release` folder.

## Disclaimer
This software is provided for educational purposes only. The developers are not responsible for any bans, suspensions, or damages resulting from the use of this software.
