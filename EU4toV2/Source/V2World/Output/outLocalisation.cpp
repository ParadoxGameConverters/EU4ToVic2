#include <ostream>
#include "../Localisation/Localisation.h"

std::ostream& V2::operator<<(std::ostream& out, const Localisation& localisation)
{
	out << Localisation::convert(localisation.tag);
	for (const auto& localisedName: localisation.name)
	{
          out << ';' << Localisation::convert(localisedName);
	}
	out << "x\n";

	out << Localisation::convert(localisation.tag) << "_ADJ";
	for (const auto& localisedAdjective: localisation.adjective)
	{
          out << ';' << Localisation::convert(localisedAdjective);
	}
	out << "x\n";

	for (const auto& party: localisation.parties)
	{
		out << Localisation::convert(party.key);
		for (const auto& localisedPartyName : party.name)
		{
			out << ';' << Localisation::convert(localisedPartyName);
		}
		out << "x\n";
	}

	return out;
}
