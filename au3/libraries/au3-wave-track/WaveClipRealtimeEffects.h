/**********************************************************************

  Audacity: A Digital Audio Editor

  WaveClipRealtimeEffects.h

  (c) 2024 Audacity Team

**********************************************************************/

#ifndef __AUDACITY_WAVECLIP_REALTIME_EFFECTS__
#define __AUDACITY_WAVECLIP_REALTIME_EFFECTS__

#include "WaveClip.h"
#include "au3-realtime-effects/RealtimeEffectList.h"
#include "au3-xml/XMLWriter.h"
#include "au3-xml/XMLAttributeValueView.h"
#include <vector>
#include <mutex>

//! Adapter class to attach RealtimeEffectList to a WaveClip
class WAVE_TRACK_API WaveClipRealtimeEffects final
    : public RealtimeEffectList
    , public WaveClipListener
{
public:
    WaveClipRealtimeEffects(WaveClip& clip);
    ~WaveClipRealtimeEffects() override;

    // WaveClipListener implementation
    void MarkChanged() noexcept override;
    void Invalidate() override;
    void WriteXMLAttributes(XMLWriter& writer) const override;
    void WriteXMLTags(XMLWriter& writer) const override;
    bool HandleXMLAttribute(
        const std::string_view& attr, const XMLAttributeValueView& valueView) override;
    XMLTagHandler* HandleXMLChild(const std::string_view& tag) override;
    void MakeStereo(WaveClipListener&& other, bool aligned) override;
    void SwapChannels() override;
    void Erase(size_t index) override;

    // ClientData implementation
    std::unique_ptr<ClientData::Cloneable<WaveClipListener>> Clone() const override;

    static RealtimeEffectList& Get(WaveClip& clip);
    static const RealtimeEffectList& Get(const WaveClip& clip);

    // Helpers to access the concrete adapter type
    static WaveClipRealtimeEffects& GetAdapter(WaveClip& clip);
    static WaveClipRealtimeEffects& GetAdapter(const WaveClip& clip);

    // Runtime processing methods
    void Initialize(double rate, size_t bufferSize);
    bool IsActive() const;
    bool GetSamples(size_t iChannel, samplePtr buffer, sampleFormat format, sampleCount start, size_t len);

private:
    WaveClip& mClip;

    struct Cache {
        sampleCount start = -1;
        size_t len = 0;
        // Float buffers for all channels of the clip
        std::vector<std::vector<float>> buffers;
    } mCache;

    // Helper to resize cache buffers
    void PrepareCache(size_t numChannels, size_t len);

    mutable std::mutex mCacheMutex;
};

#endif
