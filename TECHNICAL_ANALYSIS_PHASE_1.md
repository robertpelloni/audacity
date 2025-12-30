# Technical Analysis: Phase 1.1 - Non-Destructive Editing

## Current Architecture Analysis

In Audacity, the `WaveTrack::HandleClear` method controls how audio is deleted. It currently has two distinct paths:

1.  **Destructive Path (`split = false` and `clearByTrimming = false`)**:
    *   Used by standard "Delete" / "Cut".
    *   Calls `WaveClip::Clear(t0, t1)`.
    *   This calls `Sequence::Delete`, which removes `SampleBlocks` from the `Sequence`.
    *   **Result**: The underlying audio data structure is modified. The deleted audio is gone from the clip's reference (though preserved in Undo history).

2.  **Non-Destructive Path (`split = true` or `clearByTrimming = true`)**:
    *   Used by "Split Delete".
    *   Instead of modifying the `Sequence`, it creates two copies of the clip.
    *   It uses `TrimRight` on the first copy (Left part) and `TrimLeft` on the second copy (Right part).
    *   **Result**: The audio data remains in the `Sequence` but is hidden by trim values. This is effectively "Virtual Editing".

## The "Gap" to Audition Parity

Adobe Audition (in Multitrack) and other DAWs default to the **Non-Destructive Path**. When you delete a region:
*   The clip splits.
*   The unwanted section is hidden (trimmed out).
*   The user sees two clips (or a crossfaded join).

Audacity currently defaults to the **Destructive Path** in Waveform view to simulate a "Tape Splicing" workflow where the result is a single, continuous piece of audio.

## Proposed Implementation Strategy

To achieve "Virtual Clips" (Phase 1.1), we need to shift the default behavior of `HandleClear` (and thus `Clear`) to prefer the Non-Destructive path.

### 1. Unified "Smart Cut" Logic
We should modify `WaveTrack::Clear` or introduce a preference "Non-Destructive Editing" that forces `split = true` logic even for standard deletions.

However, simply setting `split = true` results in two separate clips. To maintain the *feel* of a single track (Audacity's strength), we might need:
*   **Heal / Merge**: If two clips are adjacent and reference the same underlying sequence with matching offsets, they should render as one continuous wave (or be auto-grouped).
*   **Container Clips**: A `WaveClip` that contains multiple child clips (Sequences) but presents as one. Audacity has `mCutLines` which is similar, but specialized.

### 2. Immediate Action Item
For the "Roadmap Implementation", the first step is to verify if we can simply use `Trim` logic for all edge-case deletions.

`WaveTrack::HandleClear` already does this:
```cpp
if (clip->BeforePlayRegion(t0)) {
    // Delete from left edge -> TrimLeft
} else if (clip->AfterPlayRegion(t1)) {
    // Delete to right edge -> TrimRight
}
```
This part is **already non-destructive**!

The issue is strictly **middle deletion**:
```cpp
} else {
    // Delete in the middle...
    if (split || clearByTrimming) {
        // Create two clips with Trims (Non-Destructive)
    } else {
        // newClip->Clear(t0, t1) (Destructive)
    }
}
```

### Recommendation
We should expose a preference (or experimental flag) to force `clearByTrimming = true` in `WaveTrack::Clear`. This would instantly convert Audacity's editing engine to be non-destructive (at the cost of creating more clip objects).

## Prototype Plan
1.  Locate `WaveTrack::Clear`.
2.  Trace where `clearByTrimming` comes from.
3.  Force it to `true` (experimentally) and observe behavior.
