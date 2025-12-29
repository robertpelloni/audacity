# Handoff Log

## Session Summary
*   **Version**: Updated to 3.7.2.
*   **Submodules**: `bobcoin` and `muse_framework` updated.
*   **Features Implemented**:
    *   **Phase 1.1**: Non-Destructive Editing (Backend).
    *   **Phase 1.2**: Clip Realtime Effects (Backend + Adapter).
    *   **Phase 1.3**: Track Routing Data Model (`mRouteId`, `mPersistentId`).
*   **Current Task**: Implementing Phase 1.3 Logic (Routing in `AudioIO.cpp`).

## Next Steps
1.  **Analyze AudioIO Routing**: Determine how to inject intermediate buffers for Bus Tracks in `FillPlayBuffers`.
2.  **Implement Mixing Logic**:
    *   Identify Bus Tracks in the project.
    *   Allocate buffers for them.
    *   Route `WaveTrack` output to Bus buffers based on `GetRouteId()`.
    *   Process Bus buffers (Apply effects/volume).
    *   Mix Bus buffers to Master.
3.  **UI**: Eventually add UI for selecting Route ID.

## Active Context
*   `PlayableTrack` now has `GetRouteId()` (defaults to 0/Master) and `GetPersistentId()`.
*   `BusTrack` exists in `au3-mixer`.
*   `WaveClipRealtimeEffects` handles clip effects.
