#ifndef __AUDACITY_ADAPTIVE_NOISE_REDUCTION_EFFECT__
#define __AUDACITY_ADAPTIVE_NOISE_REDUCTION_EFFECT__

#include <vector>
#include "StatefulPerTrackEffect.h"
#include "Effect.h"

class AdaptiveNoiseReductionEffect final : public StatefulPerTrackEffect
{
public:
    static const ComponentInterfaceSymbol Symbol;

    AdaptiveNoiseReductionEffect();
    virtual ~AdaptiveNoiseReductionEffect();

    // ComponentInterface overrides
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString GetDescription() const override;
    ManualPageID ManualPage() const override;

    // Effect overrides
    EffectType GetType() const override;
    EffectFamilySymbol GetFamily() const override;

    // StatefulPerTrackEffect overrides
    std::shared_ptr<Instance> MakeInstance() const override;

private:
    class Instance;
};

#endif
