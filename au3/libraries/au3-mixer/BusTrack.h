/**********************************************************************

  Audacity: A Digital Audio Editor

  BusTrack.h

  (c) 2024 Audacity Team

**********************************************************************/

#ifndef __AUDACITY_BUSTRACK__
#define __AUDACITY_BUSTRACK__

#include "au3-playable-track/PlayableTrack.h"
#include "au3-realtime-effects/RealtimeEffectList.h"

class MIXER_API BusTrack final : public PlayableTrack
{
    struct CreateToken {};
public:
    static const char* BusTrack_tag;

    static const TypeInfo& ClassTypeInfo();

    BusTrack(CreateToken&&);
    virtual ~BusTrack();

    static std::shared_ptr<BusTrack> Create(const std::shared_ptr<TrackList>& owner);

    // Track overrides
    Track::Holder Clone(bool backup) const override;

    // PlayableTrack overrides
    bool GetMute() const override;
    bool GetSolo() const override;

    // Bus specific
    RealtimeEffectList& GetEffects();
    const RealtimeEffectList& GetEffects() const;

    // XML
    bool HandleXMLTag(const std::string_view& tag, const AttributesList& attrs) override;
    void HandleXMLEndTag(const std::string_view& tag) override;
    XMLTagHandler* HandleXMLChild(const std::string_view& tag) override;
    void WriteXML(XMLWriter& xmlFile) const override;

private:
    RealtimeEffectList mEffects;
};

ENUMERATE_TRACK_TYPE(BusTrack);

#endif
