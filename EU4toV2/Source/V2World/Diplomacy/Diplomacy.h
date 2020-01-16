#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#include "Agreement.h"
#include "../EU4toV2/Source/EU4World/Diplomacy/EU4Agreement.h"
#include <vector>
#include "../../Mappers/CountryMappings/CountryMappings.h"
#include "../V2Country.h"

namespace V2
{
	class Diplomacy
	{
	public:
		void output() const;
		void addAgreement(const Agreement& agreement) { agreements.push_back(agreement); }
		void convertDiplomacy(
		std::vector<EU4::EU4Agreement> agreements,
		const mappers::CountryMappings& countryMapper, 
		std::map<std::string, V2Country*>& countries);

	private:
		std::vector<Agreement> agreements;
	};
}

#endif // DIPLOMACY_H