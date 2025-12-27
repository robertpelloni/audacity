# Technical Analysis: Phase 1.2 - Real-time Effects on Clips

## Current Architecture Analysis

### RealtimeEffectList
*   Managed by `au3/libraries/au3-realtime-effects/RealtimeEffectList`.
*   Currently attached to `AudacityProject` (Master Effects) and `ChannelGroup` (Track Effects).
*   Attachment is handled via `AttachedProjectObjects` and `ChannelGroup::Attachments`.

### WaveClip
*   Located in `au3/libraries/au3-wave-track/WaveClip`.
*   Inherits from `ClientData::Site<WaveClip, WaveClipListener, ClientData::DeepCopying>`.
*   This means `WaveClip` supports attachments (listeners), but they must inherit from `WaveClipListener`.

## Implementation Strategy

To support Clip-Level Effects, we need to attach `RealtimeEffectList` to `WaveClip`.

### 1. Enable Attachment
`RealtimeEffectList` currently doesn't inherit from `WaveClipListener`.
We need to create an adapter or modify `RealtimeEffectList` to work with `WaveClip`.

However, `WaveClipListener` seems designed for listeners that react to changes, not necessarily for holding state data like effects. But `WaveClip` does use `ClientData::Site` which is the standard Audacity way of attaching objects.

A better approach might be to register a factory for `WaveClip` attachments, similar to how `ChannelGroup` works.

`WaveClip` inherits from:
```cpp
public ClientData::Site<WaveClip, WaveClipListener, ClientData::DeepCopying>
```

We need to define a `WaveClipEffectList` that inherits from `WaveClipListener` and holds a `RealtimeEffectList` (or inherits from it, if multiple inheritance is safe here).

### 2. Playback Integration
The audio engine (likely `Mixer` or `AudioIO` / `Graph`) needs to know about these effects.
Currently, track effects are processed likely in `RealtimeEffectManager`.

I need to find where the audio for a clip is generated/processed and inject the effect processing there.
`WaveClip::GetSamples` is the source of audio.
If we add effects processing inside `WaveClip::GetSamples`, it might work, but `GetSamples` is `const`. Effect processing usually requires mutable state (for the effect engine).

However, `RealtimeEffectState` handles the state.

A potentially safer place is in the `PlaybackPolicy` or wherever `WaveTrack::Get` calls `WaveClip::GetSamples`.

### 3. Proposed Data Structure Changes

**Step 1**: Define `ClipRealtimeEffectList` in `RealtimeEffectList.h/cpp` (or a new file).
It should implement `WaveClipListener`.

```cpp
class ClipRealtimeEffectList : public RealtimeEffectList, public WaveClipListener {
    // Implement WaveClipListener methods (clone, etc.)
}
```

**Step 2**: Register it with `WaveClip` using `WaveClip::Attachments::RegisteredFactory`.

**Step 3**: Update `WaveClip::WriteXML` / `ReadXML` to handle the new attachment (it might happen automatically if `ClientData` handles XML, but usually we need explicit handling for `RealtimeEffectList`).
`WaveClip::HandleXMLChild` calls `Attachments::FindIf`? No, it has explicit handling.
We need to check if `ClientData::Site` automatically handles XML.
In `WaveClip::HandleXMLTag`:
```cpp
            } else if (Attachments::FindIf(
                           [&](WaveClipListener& listener){
                return listener.HandleXMLAttribute(attr, value);
            }
                           )) {
```
And `HandleXMLChild`?
```cpp
XMLTagHandler* WaveClip::HandleXMLChild(const std::string_view& tag)
{
    // ...
    // Needs to handle <effects> tag
}
```

### 4. Processing Logic (The Hard Part)
We need to find where the signal flow happens.
`src/effects/effects_base/internal/realtimeeffectservice.h` seems relevant.

For now, the goal is "Enhanced Real-Time Effects Rack" - primarily the *structure* to hold them.
The actual DSP integration might be complex.

## Immediate Plan
1.  Create `ClipRealtimeEffectList` class.
2.  Attach it to `WaveClip`.
3.  Ensure it serializes (saves/loads) with the project.
4.  Verify we can add an effect to a clip (conceptually).

This prepares the data model. Rendering comes next.
