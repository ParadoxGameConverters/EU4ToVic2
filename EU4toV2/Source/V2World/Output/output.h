#ifndef OUTPUT_H
#define OUTPUT_H

#include <ostream>
#include "../Diplomacy/Agreement.h"
#include "../Army/Army.h"
#include "../Country/Country.h"
#include "../Factory/Factory.h"
#include "../Leader/Leader.h"
#include "../Localisation/Localisation.h"
#include "../Party/Party.h"
#include "../Pop/Pop.h"

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Agreement& agreement);
	std::ostream& operator<<(std::ostream& output, const Army& army);
	std::ostream& operator<<(std::ostream& output, const Country& country);
	std::ostream& operator<<(std::ostream& output, const Factory& factory);
	std::ostream& operator<<(std::ostream& output, const Leader& leader);
	std::ostream& operator<<(std::ostream& out, const Localisation& localisation);
	std::ostream& operator<<(std::ostream& output, const Party& party);
	std::ostream& operator<<(std::ostream& output, const Pop& pop);
}
namespace mappers
{
	std::ostream& operator<<(std::ostream& output, const PartyType& partyDetails);
}

#endif // OUTPUT_H