#ifndef COUNTRY_DETAILS_H
#define COUNTRY_DETAILS_H

#include "newParser.h"
#include "../../V2World/Party/Party.h"
#include <set>

namespace mappers
{
	class CountryDetails : commonItems::parser
	{
	public:
		CountryDetails() = default;
		CountryDetails(const std::string& _filename);
		CountryDetails(std::istream& theStream);

		// These values are open to ease management.
		// This is a storage container for V2::Country.
		// Some of these values are loaded from files, others are adjusted on the fly.
		std::string filename;
		std::vector<V2::Party> parties;
		std::string primaryCulture = "dummy";
		std::set<std::string> acceptedCultures;
		std::string religion = "shamanist";
		std::string government = "absolute_monarchy";
		bool civilized = false;
		bool isReleasableVassal = true;
		std::string nationalValue = "nv_order";
		int capital = 0;
		double prestige = 0;
		double plurality = 0;
		std::string techSchool = "traditional_academic";
		bool inHRE = false;
		bool holyRomanEmperor = false;
		bool celestialEmperor = false;
		double literacy = 0.0;
		std::string rulingParty;
		int upperHouseReactionary = 10;
		int upperHouseConservative = 65;
		int upperHouseLiberal = 25;
		int numFactories = 0;

		double armyInvestment = 5.0;
		double navyInvestment = 5.0;
		double commerceInvestment = 5.0;
		double industryInvestment = 5.0;
		double cultureInvestment = 5.0;
		double slaveryInvestment = 5.0;
		double upper_house_compositionInvestment = 5.0;
		double vote_franchiseInvestment = 5.0;
		double voting_systemInvestment = 5.0;
		double public_meetingsInvestment = 5.0;
		double press_rightsInvestment = 5.0;
		double trade_unionsInvestment = 5.0;
		double political_partiesInvestment = 5.0;
		double libertyInvestment = 5.0;
		double equalityInvestment = 5.0;
		double orderInvestment = 5.0;
		double literacyInvestment = 5.0;
		double reactionaryInvestment = 5.0;
		double liberalInvestment = 5.0;

	private:
		void registerKeys();
	};
}

#endif // COUNTRY_DETAILS_H
