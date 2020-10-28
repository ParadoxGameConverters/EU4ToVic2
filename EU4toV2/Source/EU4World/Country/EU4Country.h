#ifndef EU4_COUNTRY_H
#define EU4_COUNTRY_H

#include "../Army/EU4Army.h"
#include "../../Mappers/CultureGroups/CultureGroups.h"
#include "../../Mappers/UnitTypes/UnitTypeMapper.h"
#include "../../Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "Parser.h"
#include <memory>
#include <set>
#include "EU4NationalSymbol.h"
#include "GameVersion.h"
#include "../Leader/EU4Leader.h"
#include "../Relations/EU4RelationDetails.h"
#include "../Provinces/EU4Province.h"

namespace EU4
{
	// Export data for hoi4.
	struct HistoricalEntry
	{
		std::string lastDynasty;
		bool monarchy = true;
	};

	class Country: public commonItems::parser
	{
	public:
		Country() = default;
		Country(
			std::string countryTag,
			const GameVersion& theVersion,
			std::istream& theStream,
			const mappers::IdeaEffectMapper& ideaEffectMapper, 
			const mappers::CultureGroups& cultureGroupsMapper
		);
		virtual ~Country() = default;
		Country(const Country& source) = default;
		Country& operator=(const Country&) = default;
		Country(Country&&) = default;
		Country& operator=(Country&&) = default;
		
		void readFromCommonCountry(const std::string& fileName, const std::string& fullFilename); // Add any additional information available from the specified country file.
		void setLocalisationName(const std::string& language, const std::string& name);
		void setLocalisationAdjective(const std::string& language, const std::string& adjective);
		void addProvince(const std::shared_ptr<Province>& province);
		void addCore(const std::shared_ptr<Province>& core);
	   void setInHRE(const bool _inHRE) { inHRE = _inHRE; }
		void setEmperor(const bool _emperor) { holyRomanEmperor = _emperor; }
		void setCelestialEmperor(const bool _celestialEmperor) { celestialEmperor = _celestialEmperor; }
		void eatCountry(Country& target);
		void setColonialRegion(const std::string& region) { colonialRegion = region; }
		void takeArmies(std::shared_ptr<Country>);
		void clearArmies();
		void viveLaRevolution(const bool revolting) { revolutionary = revolting; }
		void setTag(const std::string& _tag) { tag = _tag; }
		void dropMinorityCultures();
		void filterLeaders();
		void resolveRegimentTypes(const mappers::UnitTypeMapper& unitTypeMapper);
		void buildManufactoryCount(const std::map<std::string, std::shared_ptr<Country>>& theCountries);
		void increaseMfgTransfer(const int increase) { mfgTransfer += increase; }

		[[nodiscard]] const auto& getTag() const { return tag; }
		[[nodiscard]] auto getCapital() const { return capital; }
		[[nodiscard]] auto getInHRE() const { return inHRE; }
		[[nodiscard]] auto getHolyRomanEmperor() const { return holyRomanEmperor; }
		[[nodiscard]] auto getCelestialEmperor() const { return celestialEmperor; }
		[[nodiscard]] const auto& getTechGroup() const { return techGroup; }
		[[nodiscard]] auto getIsolationism() const { return isolationism; }
		[[nodiscard]] const auto& getPrimaryCulture() const { return primaryCulture; }
		[[nodiscard]] const auto& getReligion() const { return religion; }
		[[nodiscard]] auto getScore() const { return score; }
		[[nodiscard]] auto getStability() const { return stability; }
		[[nodiscard]] auto getAverageAutonomy() const { return averageAutonomy; }
		[[nodiscard]] virtual double getAdmTech() const { return admTech; }
		[[nodiscard]] virtual double getDipTech() const { return dipTech; }
		[[nodiscard]] virtual double getMilTech() const { return milTech; }
		[[nodiscard]] virtual double getArmy() const { return army; }
		[[nodiscard]] virtual double getNavy() const { return navy; }
		[[nodiscard]] virtual double getCommerce() const { return commerce; }
		[[nodiscard]] virtual double getIndustry() const { return industry; }
		[[nodiscard]] virtual double getCulture() const { return culture; }
		[[nodiscard]] auto getSlavery() const { return slavery; }
		[[nodiscard]] auto getUpper_house_composition() const { return upper_house_composition; }
		[[nodiscard]] auto getVote_franchise() const { return vote_franchise; }
		[[nodiscard]] auto getVoting_system() const { return voting_system; }
		[[nodiscard]] auto getPublic_meetings() const { return public_meetings; }
		[[nodiscard]] auto getPress_rights() const { return press_rights; }
		[[nodiscard]] auto getTrade_unions() const { return trade_unions; }
		[[nodiscard]] auto getPolitical_parties() const { return political_parties; }
		[[nodiscard]] auto getLiberty() const { return liberty; }
		[[nodiscard]] auto getEquality() const { return equality; }
		[[nodiscard]] auto getOrder() const { return order; }
		[[nodiscard]] auto getLiteracy() const { return literacy; }
		[[nodiscard]] auto getReactionary() const { return reactionary; }
		[[nodiscard]] auto getLiberal() const { return liberal; }
		[[nodiscard]] auto getFlags() const { return flags; }
		[[nodiscard]] auto getPossibleDaimyo() const { return possibleDaimyo; }
		[[nodiscard]] auto getPossibleShogun() const { return possibleShogun; }
		[[nodiscard]] const auto& getGovernment() const { return government; }
		[[nodiscard]] auto isCustom() const { return customNation; }
		[[nodiscard]] auto isColony() const { return colony; }
		[[nodiscard]] auto getLibertyDesire() const { return libertyDesire; }
		[[nodiscard]] auto isRevolutionary() const { return revolutionary; }
		[[nodiscard]] auto getManufactoryCount() const { return mfgCount + mfgTransfer; }
		[[nodiscard]] const auto& getRandomName() const { return randomName; }
		[[nodiscard]] const auto& getName() const { return name; }
				
		[[nodiscard]] const auto& getReforms() const { return governmentReforms; }
		[[nodiscard]] const auto& getRelations() const { return relations; }
		[[nodiscard]] const auto& getArmies() const { return armies; }
		[[nodiscard]] const auto& getColonialRegion() const { return colonialRegion; }
		[[nodiscard]] virtual const std::set<std::string>& getNationalIdeas() const { return nationalIdeas; }
		[[nodiscard]] const auto& getHistoricalLeaders() const { return historicalLeaders; }
		[[nodiscard]] const auto& getMilitaryLeaders() const { return militaryLeaders; }
		[[nodiscard]] const auto& getHistoricalEntry() const { return historicalEntry; }
		[[nodiscard]] const auto& getNationalColors() const { return nationalColors; }
		[[nodiscard]] const auto& getEmbracedInstitutions() const { return embracedInstitutions; }
		[[nodiscard]] const auto& getAcceptedCultures() const { return acceptedCultures; }
		[[nodiscard]] const auto& getProvinces() const { return provinces; }
		[[nodiscard]] const auto& getCores() const { return cores; }
		[[nodiscard]] const auto& exportNationalIdeas() const { return nationalIdeas; }

		[[nodiscard]] std::string getName(const std::string& language) const;
		[[nodiscard]] std::string getAdjective(const std::string& language) const;
		[[nodiscard]] double getLegitimacy() const;
		[[nodiscard]] bool getCulturalUnion() const { return culturalUnion; } // this is not used at the moment.
		[[nodiscard]] bool cultureSurvivesInCores(const std::map<std::string, std::shared_ptr<Country>>& theCountries);
		[[nodiscard]] int getTotalDev() const;
		[[nodiscard]] bool hasModifier(const std::string&) const;
		[[nodiscard]] bool hasNationalIdea(const std::string&) const;
		[[nodiscard]] bool hasFlag(const std::string&) const;
		[[nodiscard]] bool hasReform(const std::string&) const;
		[[nodiscard]] int numEmbracedInstitutions() const;
		[[nodiscard]] double getManufactoryDensity() const;

	private:
		void determineJapaneseRelations();
		void determineInvestments(const mappers::IdeaEffectMapper& ideaEffectMapper);
		void determineLibertyDesire();
		void determineCulturalUnion(const mappers::CultureGroups& cultureGroupsMapper);
		void clearProvinces();
		void clearCores();

		std::string tag; // the tag for the EU4 nation
		std::vector<std::shared_ptr<Province>> provinces; // we're pointing to main province repository in provinces class
		std::vector<std::shared_ptr<Province>> cores; // ditto
		bool inHRE = false; // if this country is an HRE member
		bool holyRomanEmperor = false; // if this country is the emperor of the HRE
		bool celestialEmperor = false; // if this country is the celestial emperor
		bool culturalUnion = false;
		int capital = 0; // the EU4 province that is this nation's capital
		std::string techGroup; // the tech group for this nation
		std::vector<bool> embracedInstitutions; // the institutions this nation has embraced
		int isolationism = 1; // the isolationism of the country (for Shinto nations with Mandate of Heaven)
		std::string primaryCulture; // the primary EU4 culture of this nation
		std::string historicalPrimaryCulture;
		std::set<std::string> acceptedCultures;  // this is not used at the moment.
		std::string religion; // the accepted religion of this country
		std::string historicalReligion;
		double score = 0.0;
		double admTech = 0.0; // the admin tech of this nation
		double dipTech = 0.0; // the diplo tech of this nation
		double milTech = 0.0; // the mil tech of this nation

		double army = 0;
		double navy = 0;
		double commerce = 0;
		double industry = 0;
		double culture = 0;
		double liberty = 0;
		double equality = 0;
		double order = 0;
		double literacy = 0;
		double reactionary = 0;
		double liberal = 0;
		
		double slavery = 0;
		double upper_house_composition = 0;
		double vote_franchise = 0;
		double voting_system = 0;
		double public_meetings = 0;
		double press_rights = 0;
		double trade_unions = 0;
		double political_parties = 0;

		std::set<std::string> flags; // any flags set for this country
		std::map<std::string, bool> modifiers; // any modifiers set for this country
		bool possibleDaimyo = false; // if this country is possibly a daimyo
		bool possibleShogun = false; // if this country is the shogun
		std::vector<Leader> historicalLeaders; // Historical leader information
		std::set<int> activeLeaderIDs; // Ones currently in service, others presumed dead.
		std::vector<Leader> militaryLeaders; // filtered active leaders
		std::string government = "monarchy";
		int governmentRank = 0;
		int development = 0;
		std::map<std::string, EU4RelationDetails> relations; // the relations with other nations
		std::vector<EU4Army> armies; // both armies and navies
		std::set<std::string> nationalIdeas; // the national ideas for this country
		double legitimacy = 100; // country at 100 doesn't store the value at all for any of the legitimacy-type values
		double republicanTradition = 100;
		double devotion = 100;
		double hordeUnity = 100;
		double stability = 0;
		double averageAutonomy = 0; // relevant for non-state consciousness in vic2
		bool customNation = false; // whether or not this is a custom or random nation
		bool colony = false; // whether or not this country is a colony
		std::string overlord;
		std::string colonialRegion; // the colonial region, if this country is a colony
		double libertyDesire = 0.0; // the amount of liberty desire
		std::string randomName; // the new name of this nation in Random World
		bool revolutionary = false; // does this country wave the glorious tri-colored banner of the revolution
		std::set<std::string> governmentReforms;
		int mfgCount = 0;
		int mfgTransfer = 0;

		// Localization attributes
		std::string	name; // the name of this country
		std::string	adjective; // the adjective for this country

		// Colors
		NationalSymbol nationalColors;

		std::map<std::string, std::string> namesByLanguage; // the names of this country in different localizations
		std::map<std::string, std::string> adjectivesByLanguage;	// the adjectives for this country in different localizations
		std::map<std::string, double> states; // Areas which have been made states by this country.

		HistoricalEntry historicalEntry;
	};
}

#endif // EU4_COUNTRY_H
