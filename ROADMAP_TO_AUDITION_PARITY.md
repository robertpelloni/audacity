# Roadmap to Audacity <> Adobe Audition Parity

This roadmap outlines the strategic phases required to bring Audacity to feature parity with Adobe Audition.

## Phase 1: The Core Foundation (Architecture & Workflow)

### 1.1 Non-Destructive Waveform Editing
**Status: Backend Complete / UI Pending**
*   **Virtual Clips**: Backend logic in `WaveTrack::HandleClear` implemented (`NonDestructiveEditing` pref).
*   **Clip Layers**: Not started.
*   **Undo History Persistence**: Not started.

### 1.2 Enhanced Real-Time Effects Rack
**Status: Backend Complete / UI Pending**
*   **Pre/Post Fader Slots**: Backend implemented (`mSplitPoint`, AudioIO processing split). UI pending.
*   **Clip-Level Effects**: Backend implemented (`WaveClipRealtimeEffects` adapter, AudioIO integration). UI pending.
*   **Sidechaining**: Not started.
*   **Effect Chain Presets**: Not started.

### 1.3 Professional Multitrack Mixing
**Status: Backend Complete / UI Pending**
*   **Bussing**: `BusTrack` scaffold and creation logic (`NEW_BUS_TRACK` action) implemented.
*   **Routing Matrix**: Data model (`PlayableTrack::mRouteId`, `mPersistentId`) and Engine logic (`AudioIO::mBusBuffers`) implemented.
*   **Automation Lanes**: Not started.

## Phase 2: The "Cool" Restoration & Analysis

### 2.1 Spectral Editing Evolution
**Status: Backend Complete / UI Pending**
*   **Spectral Spot Healing Brush**: Backend implemented (`SpectralHealEffect` with STFT/Inpainting). UI interaction pending.
*   **Lasso Selection**: Not started.
*   **Frequency Space Editing**: Not started.

### 2.2 Advanced Analysis Tools
**Status: Not Started**
*   **Phase Analysis Tool**: Not started.
*   **3D Spectral Decay**: Not started.
*   **Loudness Radar**: Not started.

### 2.3 Noise Reduction Suite
**Status: Not Started**
*   **Adaptive Noise Reduction**: Not started.
*   **DeHummer / DeClicker**: Not started.

## Phase 3: The "Pro" Production Features
*   **Loop-Based Composition**: Not started.
*   **Post-Production Tools**: Not started.

## Phase 4: Modernization & Ecosystem
*   **AI & Machine Learning**: Not started.
*   **Usability**: Not started.
