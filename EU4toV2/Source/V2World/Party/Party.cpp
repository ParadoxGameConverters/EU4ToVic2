#include "Party.h"

V2::Party::Party(const mappers::PartyType& partyType): partyDetails(partyType), name(partyType.getName()){}

bool V2::Party::isActiveOn(date query) const
{
	return query > partyDetails.getStartDate() && query < partyDetails.getEndDate();
}
