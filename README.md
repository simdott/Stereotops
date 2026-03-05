# Stereotops LV2 Plugin

Single-knob stereo width control. Range from mono to enhanced stereo.

## Features

- Single-parameter stereo width control
- Range : mono (0%) to enhanced stereo (100%)
- Default 50% position = natural stereo (no change)
- Simple, one-knob operation
- No dependencies beyond LV2 SDK

## Project Home

<https://simdott.github.io/stereotops>

## Plugin URI

`urn:simdott:stereotops`

## Dependencies

- C compiler (gcc, clang, etc.)
- LV2 development headers

### Installation by distribution

**Debian/Ubuntu** :
sudo apt-get install build-essential lv2-dev

**Fedora** :
sudo dnf install gcc lv2-devel

**Arch** :
sudo pacman -S base-devel lv2

## Build and Install

1. Download the source :
   git clone https://github.com/simdott/stereotops
   cd stereotops

2. Make the install script executable :
   chmod +x install.sh

3. Install for current user (recommended) :
   ./install.sh
   
   Or install system-wide (requires sudo) :
   sudo ./install.sh

## Verification

List installed plugins :
lv2ls | grep stereotops

Should show : `urn:simdott:stereotops`

## Usage

Load in any LV2-compatible host (Ardour, Carla, Reaper, etc.). Connect stereo inputs/outputs. 

The Stereo Width control adjusts the stereo image:
- **0%** : Mono (left and right channels summed)
- **50%** : Natural stereo (bypass, no change)
- **100%** : Enhanced stereo (widest image)

**Note:** Higher width settings will sound quieter if your track is later converted to mono. This is normal—use this plugin on tracks that will remain in stereo.

**Interface** : this plugin has no custom graphical interface. It uses your host's standard control UI (slider, knob, or numerical entry).

## Files

- stereotops.c - Plugin source code
- stereotops.ttl - Plugin description (ports, properties)
- manifest.ttl - Bundle manifest
- install.sh - Build and install script
- uninstall.sh - Uninstall script

## Uninstall

1. Make the uninstall script executable :
   chmod +x uninstall.sh

2. Uninstall for current user :
   ./uninstall.sh
   
   Or uninstall system-wide :
   sudo ./uninstall.sh
   
### v1.0.0 (2026-03-03)

- Initial release   

## License

GPL-2.0-or-later

## Author

Simon Delaruotte (simdott) 
