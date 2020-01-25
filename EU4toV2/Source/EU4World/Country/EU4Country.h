#ifndef EU4_COUNTRY_H
#define EU4_COUNTRY_H

#include "../Army/EU4Army.h"
#include "../../Mappers/CultureGroups/CultureGroups.h"
#include "../../Mappers/UnitTypes/UnitTypeMapper.h"
#include "../../Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "newParser.h"
#include <memory>
#include <optional>
#include <set>
#include "EU4NationalSymbol.h"
#include "../EU4Version.h"
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
			const std::string& countryTag,
			const Version& theVersion,
			std::istream& theStream,
			const mappers::IdeaEffectMapper& ideaEffectMapper, 
			const mappers::CultureGroups& cultureGroupsMapper
		);

		// Add any additional information available from the specified country file.
		void readFromCommonCountry(const std::string& fileName, const std::string& fullFilename);

		void setLocalisationName(const std::string& language, const std::string& name);
		void setLocalisationAdjective(const std::string& language, const std::string& adjective);

		void addProvince(const std::shared_ptr<Province>& province);
		void addCore(const std::shared_ptr<Province>& core);
	   void setInHRE(bool _inHRE) { inHRE = _inHRE; }
		void setEmperor(bool _emperor) { holyRomanEmperor = _emperor; }
		void setCelestialEmperor(bool _celestialEmperor) { celestialEmperor = _celestialEmperor; }
		bool hasModifier(std::string) const;
		bool hasNationalIdea(std::string) const;
		std::set<std::string> exportNationalIdeas() const { return nationalIdeas; }
		bool hasFlag(std::string) const ;
		void resolveRegimentTypes(const mappers::UnitTypeMapper& utm);
		int getManufactoryCount() const;
		int numEmbracedInstitutions() const;
		void eatCountry(std::shared_ptr<Country> target);
		void setColonialRegion(const std::string& region) { colonialRegion = region; }
		void takeArmies(std::shared_ptr<Country>);
		void clearArmies();
		const void viveLaRevolution(bool revolting) { revolutionary = revolting; }

		bool cultureSurvivesInCores(const std::map<std::string, std::shared_ptr<EU4::Country>>& theCountries);

		std::string getTag() const { return tag; }
		void setTag(std::string _tag) { tag = _tag; }
		std::vector<std::shared_ptr<Province>>& getProvinces() { return provinces; }
		std::vector<std::shared_ptr<Province>>& getCores() { return cores; }
		int getTotalDev() const;
		int getCapital() const { return capital; }
		bool getInHRE() const { return inHRE; }
		bool getHolyRomanEmperor() const { return holyRomanEmperor; }
		bool getCelestialEmperor() const { return celestialEmperor; }
		std::string getTechGroup() const { return techGroup; }
		const std::vector<bool>& getEmbracedInstitutions() const { return embracedInstitutions; }
		int getIsolationism() const { return isolationism; }
		std::string getPrimaryCulture() const { return primaryCulture; }
		std::vector<std::string> getAcceptedCultures() const { return acceptedCultures; }
		std::optional<mappers::CultureGroup> getCulturalUnion() const { return culturalUnion; }
		std::string getReligion() const { return religion; }
		double getScore() const { return score; }
		double getStability() const { return stability; }
		double getAverageAutonomy() const { return averageAutonomy; }
		double getLegitimacy() const;
		double getAdmTech() const { return admTech; }
		double getDipTech() const { return dipTech; }
		double getMilTech() const { return milTech; }
		double getArmy() const { return army; }
		double getNavy() const { return navy; }
		double getCommerce() const { return commerce; }
		double getIndustry() const { return industry; }
		double getCulture() const { return culture; }
		double getSlavery() const { return slavery; }
		double getUpper_house_composition() const { return upper_house_composition; }
		double getVote_franchise() const { return vote_franchise; }
		double getVoting_system() const { return voting_system; }
		double getPublic_meetings() const { return public_meetings; }
		double getPress_rights() const { return press_rights; }
		double getTrade_unions() const { return trade_unions; }
		double getPolitical_parties() const { return political_parties; }
		double getLiberty() const { return liberty; }
		double getEquality() const { return equality; }
		double getOrder() const { return order; }
		double getLiteracy() const { return literacy; }
		double getReactionary() const { return reactionary; }
		double getLiberal() const { return liberal; }
		bool getPossibleDaimyo() const { return possibleDaimyo; }
		bool getPossibleShogun() const { return possibleShogun; }
		std::string getGovernment() const { return government; }
		const std::set<std::string>& getReforms() const { return governmentReforms; }
		const std::map<std::string, EU4RelationDetails>& getRelations() const { return relations; }
		const std::vector<EU4Army>& getArmies() const { return armies; }
		bool isCustom() const { return customNation; }
		bool isColony() const { return colony; }
		const std::string& getColonialRegion() const { return colonialRegion; }
		double getLibertyDesire() const { return libertyDesire; }
		bool isRevolutionary() const { return revolutionary; }
		std::string getRandomName() const { return randomName; }
		const std::set<std::string>& getNationalIdeas() const { return nationalIdeas; }
		const std::vector<Leader>& getHistoricalLeaders() const { return historicalLeaders; }
		const std::vector<Leader>& getMilitaryLeaders() const { return militaryLeaders; }

		std::string	getName() const { return name; }
		std::string	getName(const std::string& language) const;
		std::string	getAdjective(const std::string& language) const;
		void dropMinorityCultures();
		void filterLeaders();
		const HistoricalEntry& getHistoricalEntry() const { return historicalEntry; }

		const NationalSymbol& getNationalColors() const { return nationalColors; }

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
		int capital = 0; // the EU4 province that is this nation's capital
		std::string techGroup; // the tech group for this nation
		std::vector<bool> embracedInstitutions; // the institutions this nation has embraced
		int isolationism = 1; // the isolationism of the country (for Shinto nations with Mandate of Heaven)
		std::string primaryCulture; // the primary EU4 culture of this nation
		std::vector<std::string> acceptedCultures; // the accepted EU4 cultures for this nation
		std::optional<mappers::CultureGroup> culturalUnion;
		std::string religion; // the accepted religion of this country
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

		std::map<std::string, bool> flags; // any flags set for this country
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
		bool revolutionary = false; // does this country wave the glorious tricoloured banner of the revolution
		std::set<std::string> governmentReforms;

		// Localisation attributes
		std::string	name; // the name of this country
		std::string	adjective; // the adjective for this country

		// Colors
		NationalSymbol nationalColors;

		std::map<std::string, std::string> namesByLanguage; // the names of this country in different localisations
		std::map<std::string, std::string> adjectivesByLanguage;	// the adjectives for this country in different localisations
		std::map<std::string, double> states; // Areas which have been made states by this country.

		HistoricalEntry historicalEntry;
	};
}

#endif // EU4_COUNTRY_H
