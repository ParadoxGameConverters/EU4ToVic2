#include "output.h"

std::ostream& V2::operator<<(std::ostream& output, const Localisation& localisation)
{
	output << Localisation::convert(localisation.tag);
	for (const auto& localisedName: localisation.name)
	{
		output << ';' << Localisation::convert(localisedName);
	}
	output << "x\n";

	output << Localisation::convert(localisation.tag) << "_ADJ";
	for (const auto& localisedAdjective: localisation.adjective)
	{
		output << ';' << Localisation::convert(localisedAdjective);
	}
	output << "x\n";

	for (const auto& party: localisation.parties)
	{
		output << Localisation::convert(party.key);
		for (const auto& localisedPartyName : party.name)
		{
			output << ';' << Localisation::convert(localisedPartyName);
		}
		output << "x\n";
	}

	return output;
}
