# Technical Analysis: Phase 1.3 - Professional Mixing & Bussing

## Goals
Audition features a robust "Mixer" view with faders, EQ, inserts, and Sends. It also supports "Bus" tracks (Aux tracks) for grouping audio.
Audacity's current "Mixer Board" is a separate, limited view.

## Current Architecture
*   `WaveTrack`: The primary audio container. Contains `WaveClip`s.
*   `Mixer`: A class that processes `WaveTrack`s into a stereo/mono stream.
*   `ChannelGroup`: Base class for `WaveTrack` (and others), handling linked channels.
*   `AudioIO`: Manages playback streams.

## Missing Concepts
1.  **Bus Track**: A track that contains no clips but receives audio from other tracks.
2.  **Routing**: The ability to send the output of a `WaveTrack` to a `BusTrack` instead of the Master Output.
3.  **Sends**: A split signal flow (Pre/Post fader) to a Bus (typically for Reverb/Delay).

## Proposed Architecture for Bussing

### 1. `BusTrack` Class
Create a new class `BusTrack` inheriting from `PlayableTrack` (or `ChannelGroup` directly if it doesn't need to be "playable" in the timeline sense, but it needs to be in the Mixer).
It needs:
*   `RealtimeEffectList` (Inserts).
*   Volume/Pan controls.
*   Input Buffers (summing point).

**Status:** Scaffold implemented in `au3/libraries/au3-mixer/BusTrack.h`. Type Info registered.

### 2. Routing Logic
Currently, `AudioIO` iterates `mPlaybackTracks` and mixes them to `mMasterBuffers`.
We need an intermediate mixing stage.

**Old Flow:**
`WaveTrack` -> `Mixer` -> `MasterBuffer` -> Output

**New Flow:**
`WaveTrack` -> `Route` -> `BusTrack` -> `MasterBuffer` -> Output

We need a `RoutingGraph` or `MixerGraph`.

### 3. Implementation Steps
1.  **Define `BusTrack`**: New library `au3-bus-track`? Or inside `au3-mixer`? (Done: inside `au3-mixer`).
2.  **Routing Property**: Add `DestinationID` to `WaveTrack`. Default is `Master`. (Done: `PlayableTrack` has `mRouteId` and `mPersistentId`).
3.  **Audio Engine Update**: Modify `AudioIO::FillPlayBuffers` (specifically `ProcessPlaybackSlices`) to:
    *   Process all `WaveTrack`s (apply clip effects).
    *   Instead of summing to `MasterBuffer`, sum to the buffer of the target `BusTrack`.
    *   Process `BusTrack`s (apply bus effects).
    *   Sum `BusTrack` outputs (and tracks routed to Master) to `MasterBuffer`.

    **Status:** Implemented. `AudioIO` now handles `mBusTracks`, allocates `mBusBuffers`, routes output, and processes busses.

## Risks & Challenges
*   **Latency Compensation**: Busses introduce routing complexity. If a track goes to Master AND a Bus (Send), latency must be aligned.
*   **Feedback Loops**: Need detection to prevent Bus A -> Bus B -> Bus A. (Not yet implemented).
*   **UI**: The Track Panel UI needs to show busses. They should probably sit at the bottom or in a separate folder.

## Next Steps
*   **UI**: Add "Add Bus Track" menu item.
*   **UI**: Add Routing Selector (Dropdown in Track Control Panel) to set `RouteId`.
*   **Sends**: Implement Pre/Post Fader Sends.
