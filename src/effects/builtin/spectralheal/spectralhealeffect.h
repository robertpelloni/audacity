/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "au3-effects/Effect.h"

namespace au::effects {

class SpectralHealEffect final : public Effect
{
public:
    static const ComponentInterfaceSymbol Symbol;

    SpectralHealEffect();
    virtual ~SpectralHealEffect();

    // ComponentInterface
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString GetDescription() const override;
    ManualPageID ManualPage() const override;

    // EffectDefinitionInterface
    EffectType GetType() const override;
    EffectFamilySymbol GetFamily() const override;
    bool IsInteractive() const override;

    // Effect
    bool Process() override;
};

} // namespace au::effects
