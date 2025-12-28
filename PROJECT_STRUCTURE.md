# Project Structure & Dashboard

## Submodules

| Path | URL | Status | Version (Commit) | Date Checked |
|------|-----|--------|------------------|--------------|
| `muse_framework` | `https://github.com/musescore/framework_tmp.git` | Active | `db0f4fd338c51996a72ea2fa05cc687e3686f1ec` | 2025-12-28 |

## Directory Layout

*   **`au3/`**: Core Audacity 3+ libraries and modules.
    *   **`libraries/`**: Reusable C++ libraries (modular architecture).
        *   `au3-wave-track`: Core waveform handling.
        *   `au3-realtime-effects`: Real-time effect chain management.
        *   `au3-audio-io`: Audio playback and recording engine.
        *   `au3-project`: Project management.
    *   **`modules/`**: Optional modules (importers, cloud sharing).
    *   **`src/`**: Main application logic (UI, Preferences, Menus).
*   **`src/`**: Legacy/Main source directory (gradually moving to `au3`).
*   **`muse_framework/`**: UI framework submodule.

## Build Information

*   **Build System**: CMake
*   **Compiler Standard**: C++17 (implied)

## Recent Feature Additions (Audition Parity Roadmap)

*   **Non-Destructive Editing**: Experimental preference enabled in `TracksBehaviorsPrefs`.
*   **Clip-Level Realtime Effects**: Infrastructure added to `WaveClip` to support effect chains.
*   **Mixing Architecture Analysis**: `TECHNICAL_ANALYSIS_PHASE_1_3.md` added.
