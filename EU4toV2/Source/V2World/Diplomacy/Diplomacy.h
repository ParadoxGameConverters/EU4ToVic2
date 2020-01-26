#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#include "Agreement.h"
#include "../EU4toV2/Source/EU4World/Diplomacy/EU4Agreement.h"
#include <vector>
#include "../../Mappers/CountryMappings/CountryMappings.h"
#include "../../Mappers/AgreementMapper/AgreementMapper.h"

namespace V2
{
	class Relation;
	class Country;
	class Diplomacy
	{
	public:
		void output() const;
		void addAgreement(const Agreement& agreement) { agreements.push_back(agreement); }
		void convertDiplomacy(
			std::vector<EU4::EU4Agreement> agreements,
			const mappers::CountryMappings& countryMapper, 
			std::map<std::string, std::shared_ptr<Country>>& countries);

	private:
		std::vector<Agreement> agreements;

		mappers::AgreementMapper agreementMapper;
		std::map<std::string, int> conglomerateCache;
		std::map<std::string, int> vassalCache;
		std::map<std::string, int> masterCache;
		std::map<std::string, std::set<std::string>> masterVassals;

		static void convertRelationsToInfluence(const std::map<std::string, std::shared_ptr<Country>>& countries);
		void reduceVassalPrestige(const std::map<std::string, std::shared_ptr<Country>>& countries);
		static void processVassal(Relation& r1, Relation& r2);
		static void processTributary(Relation& r1, Relation& r2);
		static void processOnesider(Relation& r1);
		static void processDoublesider(Relation& r1, Relation& r2);
		void storeDevValues(const Country& country1, const Country& country2);
	};
}

#endif // DIPLOMACY_H