# Project Dashboard & Structure

## Version
Current Version: 3.7.2

## Submodules
| Name | Path | Version (Commit) | Description |
| :--- | :--- | :--- | :--- |
| **bobcoin** | `bobcoin/` | `44f3944e89a5eba5f95113b56709bf7500265e04` | Bobcoin cryptocurrency submodule. |
| **muse_framework** | `muse_framework/` | `db0f4fd338c51996a72ea2fa05cc687e3686f1ec` | MuseScore UI Framework (Qt/QML based). |

## Directory Structure
*   `au3/`: Core Audacity 3 libraries (Modern C++ architecture).
    *   `au3-wave-track/`: Waveform handling, Clips, Rendering.
    *   `au3-mixer/`: Mixing engine, `BusTrack`, `Envelope`.
    *   `au3-audio-io/`: PortAudio backend, playback scheduling.
    *   `au3-realtime-effects/`: VST/AU/LV2 hosting and processing.
    *   `au3-playable-track/`: `PlayableTrack` base class (Mute/Solo/Routing).
*   `src/`: Application logic, Menus, Commands (Legacy & GUI).
*   `modules/`: Dynamic modules (Scripting, etc).
*   `include/`: Global headers.

## Build System
*   **CMake**: Primary build system.
*   **Conan**: Dependency management (optional/legacy).
*   **Pre-requisites**: C++17, Python 3, CMake 3.16+, wxWidgets 3.1+, Qt 6 (for Muse Framework).

## Key Files
*   `VERSION.md`: Single source of truth for project version.
*   `CHANGELOG.txt`: History of changes.
*   `ROADMAP_TO_AUDITION_PARITY.md`: Strategic roadmap.
*   `LLM_INSTRUCTIONS.md`: Universal instructions for AI Agents.
