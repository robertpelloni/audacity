# Technical Analysis: Phase 1.2 - Real-time Effects on Clips

## Status: Backend Completed

### Accomplished
1.  **Data Structure**: Created `WaveClipRealtimeEffects` adapter (inheriting `RealtimeEffectList` and `WaveClipListener`).
2.  **Attachment**: Attached to `WaveClip` via `WaveClip::Attachments`.
3.  **Persistence**: XML serialization implemented (fixed critical issue with attributes vs tags).
4.  **Processing Logic**:
    *   `WaveClipRealtimeEffects` implements a caching mechanism to synchronize stereo processing from sequential `GetSamples` calls.
    *   `WaveTrack::GetOne` intercepts sample retrieval and delegates to `WaveClipRealtimeEffects::GetSamples`.
    *   `AudioIO` initializes clip effects in `TransportState`.
5.  **Pre/Post Fader Support**:
    *   `RealtimeEffectList` supports `Stage` (Pre/Post) split point.
    *   `AudioIO` applies Pre-Fader effects, then Volume (Gain), then Post-Fader effects, then Pan (Mix).

### Remaining
*   **UI**: Expose Clip Effects in the Effects Rack UI (EffectRack logic needs to know about selection/focus clip).
*   **UI**: Expose Pre/Post split point in UI.
