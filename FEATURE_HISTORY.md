# Feature History: Cool Edit Pro to Adobe Audition

This document tracks the evolution of features from Cool Edit Pro through the various versions of Adobe Audition. It serves as a reference for the "Audacity to Audition" parity roadmap.

## Cool Edit Pro (Syntrillium Software)

### Cool Edit Pro 2.0 / 2.1
The definitive version before Adobe acquisition. Known for its lightweight footprint and powerful destructive editing + non-destructive multitrack.

*   **Core Audio Engine**:
    *   Sample rates up to 10MHz, 32-bit floating point depth.
    *   Destructive Waveform View (Single file).
    *   Non-destructive Multitrack View (128 tracks).
*   **Multitrack Features**:
    *   Real-time non-destructive effects on tracks.
    *   Track Grouping / Bussing (combining tracks to a virtual bus).
    *   Loop-based composition (Loopology) with beat/key matching.
    *   Volume/Pan envelopes on clips.
    *   "Lock" track to pre-process effects (freeze).
    *   Basic MIDI file support (playback only).
*   **Analysis & Restoration**:
    *   **Phase Analysis** tool.
    *   **Click/Pop Eliminator** with auto-find, spectral verification.
    *   **Hiss Reduction** with noise floor graphing/learning.
    *   **Clip Restoration** (reconstructs clipped peaks).
    *   **Spectral View**: 256 bands, color-coded amplitude.
*   **Key Effects**:
    *   *Full Reverb*: Convolution-based simulation of acoustic spaces.
    *   *Hard Limiter*: Boost input and limit max amplitude.
    *   *Dynamics Processing*: Graphic editing of dynamic range (compressor/expander/gate).
    *   *Pitch Bender*: Graph-based tempo/pitch variation over time.
    *   *DTMF/Notch Filter*: Remove specific frequencies (hum removal).
    *   *Vocoder*: Modulate one signal with another.
    *   *Envelope Follower*: Apply amplitude envelope of one wave to another.
*   **Other**:
    *   CD Ripping and "Extract Audio from CD".
    *   CD Burning (Track-at-once).
    *   Scripting/Batch Processing.

## Adobe Audition (The "Classic" Era)

### Adobe Audition 1.0 (2003)
*   Essentially a rebranded Cool Edit Pro 2.1.
*   No major feature changes, primarily stability and branding.

### Adobe Audition 1.5 (2004)
*   **Pitch Correction**: Auto-tune style pitch correction effect.
*   **Frequency Space Editing**: Ability to select and edit specific frequencies in Spectral View (Marquee selection).
*   **CD Project View**: Dedicated view for mastering audio CDs.
*   **Video Support**: Basic video thumbnail track and import for scoring.
*   **ReWire Support**: Integration with Reason/Ableton.
*   **Clip Time Stretching**: Integrated time-stretching tool.

### Adobe Audition 2.0 (2006) - The "Pro" Rewrite
*   **New UI**: Dockable panels, dark workspace (professional look).
*   **ASIO Support**: Low-latency driver support.
*   **VST Support**: Native VST plugin support (replacing some DirectX/native dependencies).
*   **Multitrack Improvements**:
    *   Unlimited tracks.
    *   Real-time clip stretching.
    *   Automation lanes (read/write/latch/touch).
*   **Mastering Tools**: Integrated iZotope multiband compressor and limiter.
*   **Phase Correction**: Automatic phase correction tool.

### Adobe Audition 3.0 (2007) - The Fan Favorite
*   **VSTi Support**: Virtual Instrument support with MIDI sequencing (piano roll).
*   **Spectral Spot Healing**: "Photoshop for Audio" – healing brush to remove noises in spectral view.
*   **Effects**:
    *   Tube-modeled Compressor.
    *   Analog Delay.
    *   Convolution Reverb (native).
    *   Guitar Suite (amp simulation).
*   **Workflow**:
    *   Top/Tail view.
    *   Batch processing improvements.
    *   Radius time stretching (iZotope algorithm).

## Adobe Audition (The Cross-Platform "Rewrite" Era)

### Adobe Audition CS5.5 (v4.0) (2011)
*   **Architecture**: Rewritten for macOS compatibility (Cocoa).
*   **Removals**: MIDI sequencing, CD burning, and some legacy effects removed.
*   **Audio Engine**: New playback engine, faster performance.
*   **Surround Sound**: Native 5.1 multichannel support.
*   **Exchange**: OMF and XML interchange with Premiere Pro/FCP.
*   **New Effects**: DeHummer, DeEsser, Speech Volume Leveler.

### Adobe Audition CS6 (v5.0) (2012)
*   **Clip Features**: Real-time clip stretching, Clip grouping restored.
*   **Automatic Speech Alignment**: Syncing ADR/dubbing to original audio.
*   **Control Surface Support**: EUCON and Mackie protocol.
*   **Pitch Correction**: Manual pitch correction (visual editor).
*   **CD Burning**: Restored.

## Adobe Audition (The "Creative Cloud" Era)

### Adobe Audition CC (v6.0+) (2013-Present)
*   **Architecture**: 64-bit support.
*   **Sound Remover**: Advanced "sound print" removal (like noise reduction but for complex sounds).
*   **Preview Editor**: Split view to see "before/after" edits visually.
*   **Remix Tool**: AI-driven song retiming (shortens songs while keeping musicality).
*   **Auto-Ducking**: Music automatically lowers when speech is detected (Essential Sound panel).
*   **Strip Silence**: Automated removal of silence in multitrack.
*   **Loudness Radar**: TC Electronic loudness meter (CALM Act compliance).
