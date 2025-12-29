/**********************************************************************

  Audacity: A Digital Audio Editor

  WaveClipRealtimeEffects.cpp

  (c) 2024 Audacity Team

**********************************************************************/

#include "WaveClipRealtimeEffects.h"
#include "au3-realtime-effects/RealtimeEffectState.h"
#include "au3-math/float_cast.h"
#include "au3-utility/MemoryX.h"
#include "au3-channel/ChannelGroup.h"
#include "au3-math/SampleFormat.h"

static const WaveClip::Attachments::RegisteredFactory sFactory{
   [](auto& owner) {
      return std::make_unique<WaveClipRealtimeEffects>(static_cast<WaveClip&>(owner));
   }
};

WaveClipRealtimeEffects::WaveClipRealtimeEffects(WaveClip& clip)
    : mClip(clip)
{
}

WaveClipRealtimeEffects::~WaveClipRealtimeEffects() = default;

void WaveClipRealtimeEffects::MarkChanged() noexcept
{
   // Notify listeners if needed
}

void WaveClipRealtimeEffects::Invalidate()
{
   std::lock_guard<std::mutex> lock(mCacheMutex);
   // Invalidate cache if needed
   mCache.start = -1;
}

void WaveClipRealtimeEffects::WriteXMLAttributes(XMLWriter& writer) const
{
}

void WaveClipRealtimeEffects::WriteXMLTags(XMLWriter& writer) const
{
   RealtimeEffectList::WriteXML(writer);
}

bool WaveClipRealtimeEffects::HandleXMLAttribute(
    const std::string_view& attr, const XMLAttributeValueView& valueView)
{
   return false;
}

XMLTagHandler* WaveClipRealtimeEffects::HandleXMLChild(const std::string_view& tag)
{
   return RealtimeEffectList::HandleXMLChild(tag);
}

void WaveClipRealtimeEffects::MakeStereo(WaveClipListener&& other, bool aligned)
{
   // Merge effects from other clip?
   // For now, we assume effects are per-clip and don't merge automatically or we keep left.
}

void WaveClipRealtimeEffects::SwapChannels()
{
    // If we support channel-specific effects, we'd swap them.
}

void WaveClipRealtimeEffects::Erase(size_t index)
{
   // Handle erasure
}

std::unique_ptr<ClientData::Cloneable<WaveClipListener>> WaveClipRealtimeEffects::Clone() const
{
   auto clone = std::make_unique<WaveClipRealtimeEffects>(mClip);
   // Copy the effect list
   // RealtimeEffectList copy constructor should handle deep copy of settings
   *static_cast<RealtimeEffectList*>(clone.get()) = *this;
   return clone;
}

RealtimeEffectList& WaveClipRealtimeEffects::Get(WaveClip& clip)
{
   return clip.Attachments::Get<WaveClipRealtimeEffects>(sFactory);
}

const RealtimeEffectList& WaveClipRealtimeEffects::Get(const WaveClip& clip)
{
   return Get(const_cast<WaveClip&>(clip));
}

WaveClipRealtimeEffects& WaveClipRealtimeEffects::GetAdapter(WaveClip& clip)
{
    return clip.Attachments::Get<WaveClipRealtimeEffects>(sFactory);
}

WaveClipRealtimeEffects& WaveClipRealtimeEffects::GetAdapter(const WaveClip& clip)
{
    // Const-cast to allow retrieving mutable adapter from const clip
    // This is safe because the adapter holds runtime state (cache, effects)
    // that doesn't affect the logical structure of the clip during reading.
    return GetAdapter(const_cast<WaveClip&>(clip));
}

void WaveClipRealtimeEffects::Initialize(double rate, size_t bufferSize)
{
    // Initialize all effects in the list
    Visit([&](RealtimeEffectState& state, bool) {
        state.Initialize(rate, bufferSize);
    });

    std::lock_guard<std::mutex> lock(mCacheMutex);
    // Reset cache
    mCache.start = -1;
    mCache.len = 0;
}

bool WaveClipRealtimeEffects::IsActive() const
{
    // Check if we have any active effects
    bool active = false;
    Visit([&](const RealtimeEffectState& state, bool listActive) {
        if (listActive && state.IsActive()) {
            active = true;
        }
    });
    return active;
}

void WaveClipRealtimeEffects::PrepareCache(size_t numChannels, size_t len)
{
    if (mCache.buffers.size() != numChannels) {
        mCache.buffers.resize(numChannels);
    }
    for (auto& buf : mCache.buffers) {
        if (buf.size() < len) {
            buf.resize(len);
        }
    }
}

bool WaveClipRealtimeEffects::GetSamples(size_t iChannel, samplePtr buffer, sampleFormat format, sampleCount start, size_t len)
{
    if (!IsActive()) {
        // Fallback should not happen if caller checked IsActive, but safety first
        return mClip.GetSamples(iChannel, buffer, format, start, len);
    }

    std::lock_guard<std::mutex> lock(mCacheMutex);

    size_t numChannels = mClip.NChannels();

    // Check cache hit
    bool cacheHit = (mCache.start == start && mCache.len >= len && mCache.buffers.size() == numChannels);

    if (!cacheHit) {
        // Refill cache
        PrepareCache(numChannels, len);

        // 1. Fetch raw samples for all channels
        std::vector<samplePtr> rawPtrs(numChannels);
        for (size_t ch = 0; ch < numChannels; ++ch) {
            rawPtrs[ch] = reinterpret_cast<samplePtr>(mCache.buffers[ch].data());
        }

        // Fetch as float directly since effects need float
        if (!mClip.GetSamples(rawPtrs.data(), floatSample, start, len)) {
            return false;
        }

        // 2. Apply effects
        std::vector<float*> processPtrs(numChannels);
        for (size_t ch = 0; ch < numChannels; ++ch) {
            processPtrs[ch] = mCache.buffers[ch].data();
        }

        // Dummy buffer for effects that need it (usually unused by Process but required by signature)
        std::vector<float> dummy(len);

        Visit([&](RealtimeEffectState& state, bool listActive) {
             if (!listActive || !state.IsActive()) return;

             // Prepare processing
             if (state.ProcessStart(true)) {
                 // We use the pointer to the clip as the group ID.
                 // RealtimeEffectState uses it only as a map key.
                 const ChannelGroup* groupKey = reinterpret_cast<const ChannelGroup*>(&mClip);

                 state.Process(groupKey, numChannels, processPtrs.data(), processPtrs.data(), dummy.data(), len);
                 state.ProcessEnd();
             }
        });

        // Update cache metadata
        mCache.start = start;
        mCache.len = len;
    }

    // 3. Serve requested channel from cache
    if (iChannel >= mCache.buffers.size()) {
        return false;
    }

    const float* src = mCache.buffers[iChannel].data();

    // Copy and convert if necessary
    CopySamples(reinterpret_cast<constSamplePtr>(src), floatSample,
                buffer, format,
                len);

    return true;
}
