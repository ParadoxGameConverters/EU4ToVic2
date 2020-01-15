#ifndef OUT_PARTY_H
#define OUT_PARTY_H

#include "../Party/Party.h"
#include "../../Mappers/PartyTypes/PartyType.h"

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Party& party);
};

namespace mappers
{
	std::ostream& operator<<(std::ostream& output, const PartyType& party);
}

#endif // OUT_PARTY_H