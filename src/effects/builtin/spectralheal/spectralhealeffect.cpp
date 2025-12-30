/*
 * Audacity: A Digital Audio Editor
 */
#include "spectralhealeffect.h"
#include "au3-effects/EffectManager.h"
#include "au3-effects/EffectOutputTracks.h"
#include "au3-wave-track/WaveTrack.h"
#include "au3-wave-track-fft/TrackSpectrumTransformer.h"
#include "au3-fft/FFT.h"
#include <algorithm>
#include <cmath>

namespace au::effects {

const ComponentInterfaceSymbol SpectralHealEffect::Symbol { XO("Spectral Heal") };

SpectralHealEffect::SpectralHealEffect()
{
}

SpectralHealEffect::~SpectralHealEffect() = default;

ComponentInterfaceSymbol SpectralHealEffect::GetSymbol() const
{
    return Symbol;
}

TranslatableString SpectralHealEffect::GetDescription() const
{
    return XO("Heals the selected spectral region using surrounding frequencies.");
}

ManualPageID SpectralHealEffect::ManualPage() const
{
    return XO("Spectral_Heal");
}

EffectType SpectralHealEffect::GetType() const
{
    return EffectTypeProcess;
}

EffectFamilySymbol SpectralHealEffect::GetFamily() const
{
    return EffectFamilySymbol{ XO("Spectral") };
}

bool SpectralHealEffect::IsInteractive() const
{
    return false;
}

namespace {
// Define Transformer
struct HealTransformer : TrackSpectrumTransformer
{
    HealTransformer(WaveChannel* pOutputTrack, size_t windowSize)
        : TrackSpectrumTransformer(pOutputTrack, true,
                                   eWinFuncHann, eWinFuncHann,
                                   windowSize, 2, // 50% overlap
                                   true, true)
    {
    }
};
}

bool SpectralHealEffect::Process()
{
    // Prepare output tracks
    EffectOutputTracks outputs { *mTracks, GetType(), { { mT0, mT1 } } };

    // Window parameters
    const size_t windowSize = 4096; // Good resolution for spectral work

    bool bGoodResult = true;

    for (auto track : outputs.Get().Selected<WaveTrack>()) {
        const double rate = track->GetRate();
        const double binSize = rate / windowSize;

        // Determine bins
        size_t minBin = 0;
        size_t maxBin = windowSize / 2;

        // mF0/mF1 are in Hz
        if (mF0 > 0.0) minBin = static_cast<size_t>(mF0 / binSize);
        if (mF1 > 0.0) maxBin = static_cast<size_t>(mF1 / binSize);

        minBin = std::clamp(minBin, (size_t)0, windowSize/2);
        maxBin = std::clamp(maxBin, (size_t)0, windowSize/2);

        // If no spectral selection, do nothing (or whole freq?)
        // If F0/F1 are 0, typically means "Select All".
        // But for "Healing", we need a target.
        // Assume if F0=F1=0 (or undefined), we process nothing or return error?
        // Audacity default is F0=F1=undefined implies time selection only.
        // Healing time selection only? Maybe remove clicks?
        // Let's assume we proceed if we have a range.
        if (minBin >= maxBin) {
             // If full range, we might just be doing a time-based interpolation/blur?
             // Or skipping.
             continue;
        }

        auto start = track->TimeToLongSamples(mT0);
        auto end = track->TimeToLongSamples(mT1);
        auto len = end - start;

        auto pOutputTrack = track->EmptyCopy();
        auto pIter = pOutputTrack->Channels().begin();

        for (const auto pChannel : track->Channels()) {
            if (pIter == pOutputTrack->Channels().end()) break;
            auto pOutChannel = (*pIter++).get();

            HealTransformer transformer(pOutChannel, windowSize);

            // Queue size needs to be enough for lookahead.
            // History Len = 3 is enough for +/- 1 window.
            unsigned historyLen = 3;

            auto processor = [&](SpectrumTransformer& trans) -> bool {
                if (trans.GetQueueSize() < historyLen) return true;

                auto& healTrans = static_cast<HealTransformer&>(trans);

                // NthWindow(0) is Newest (Future relative to 1)
                // NthWindow(1) is Target
                // NthWindow(2) is Oldest (Past relative to 1)

                auto& winFuture = healTrans.NthWindow(0);
                auto& winTarget = healTrans.NthWindow(1);
                auto& winPast = healTrans.NthWindow(2);

                // Iterate bins
                for (size_t k = minBin; k < maxBin; ++k) {
                    // Simple Healing: Average of Time Neighbors (Interpolation)
                    float rPast = winPast.mRealFFTs[k];
                    float iPast = winPast.mImagFFTs[k];
                    float rFut = winFuture.mRealFFTs[k];
                    float iFut = winFuture.mImagFFTs[k];

                    // Simple average
                    float rNew = (rPast + rFut) * 0.5f;
                    float iNew = (iPast + iFut) * 0.5f;

                    // Assign
                    winTarget.mRealFFTs[k] = rNew;
                    winTarget.mImagFFTs[k] = iNew;
                }

                return true; // Continue
            };

            if (!transformer.Process(processor, *pChannel, historyLen, start, len)) {
                bGoodResult = false;
                break;
            }
        }

        if (bGoodResult) {
            TrackSpectrumTransformer::PostProcess(*pOutputTrack, len);
            track->ClearAndPaste(mT0, mT1, *pOutputTrack, true, true);
        } else {
            break;
        }
    }

    if (bGoodResult) {
        outputs.Commit();
    }

    return bGoodResult;
}

} // namespace au::effects
