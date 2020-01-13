#ifndef COUNTRY_MAPPING_H
#define COUNTRY_MAPPING_H

#include "newParser.h"

namespace mappers
{
	class CountryMapping: commonItems::parser
	{
	public:
		CountryMapping(std::istream& theStream);

		std::string getEU4Tag() const { return EU4Tag; }
		std::string getVic2Tag() const { return Vic2Tag; }

	private:
		std::string EU4Tag;
		std::string Vic2Tag;
	};
}

#endif // COUNTRYMAPPING_H