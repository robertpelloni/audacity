/**********************************************************************

  Audacity: A Digital Audio Editor

  BusTrack.cpp

  (c) 2024 Audacity Team

**********************************************************************/

#include "BusTrack.h"
#include "au3-xml/XMLWriter.h"

const char* BusTrack::BusTrack_tag = "bustrack";

BusTrack::BusTrack(CreateToken&&)
{
}

BusTrack::~BusTrack() = default;

std::shared_ptr<BusTrack> BusTrack::Create(const std::shared_ptr<TrackList>& owner)
{
    auto track = std::make_shared<BusTrack>(CreateToken{});
    if (owner) {
        owner->Add(track);
    }
    return track;
}

 auto BusTrack::ClassTypeInfo() -> const TypeInfo&
 {
    static Track::TypeInfo info{
        { "bus", "bus", XO("Bus Track") },
        true, &PlayableTrack::ClassTypeInfo() };
    return info;
 }

Track::Holder BusTrack::Clone(bool backup) const
{
    auto newTrack = std::make_shared<BusTrack>(CreateToken{});
    newTrack->Init(*this);
    newTrack->mEffects = mEffects;
    return newTrack;
}

bool BusTrack::GetMute() const
{
    return false; // Todo
}

bool BusTrack::GetSolo() const
{
    return false; // Todo
}

RealtimeEffectList& BusTrack::GetEffects()
{
    return mEffects;
}

const RealtimeEffectList& BusTrack::GetEffects() const
{
    return mEffects;
}

bool BusTrack::HandleXMLTag(const std::string_view& tag, const AttributesList& attrs)
{
    if (tag == BusTrack_tag) {
        // Handle attributes
        return true;
    }
    return false;
}

void BusTrack::HandleXMLEndTag(const std::string_view& tag)
{
}

XMLTagHandler* BusTrack::HandleXMLChild(const std::string_view& tag)
{
    if (tag == RealtimeEffectList::XMLTag()) {
        return &mEffects;
    }
    return nullptr;
}

void BusTrack::WriteXML(XMLWriter& xmlFile) const
{
    xmlFile.StartTag(BusTrack_tag);
    // Write attributes
    mEffects.WriteXML(xmlFile);
    xmlFile.EndTag(BusTrack_tag);
}
