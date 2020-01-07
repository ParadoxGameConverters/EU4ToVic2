#ifndef EU4_COUNTRY_H_
#define EU4_COUNTRY_H_

#include "Army/EU4Army.h"
#include "Color.h"
#include "Date.h"
#include "CultureGroups.h"
#include "../Mappers/UnitTypeMapper.h"
#include "newParser.h"
#include <memory>
#include <optional>
#include <set>
#include "Country/EU4NationalSymbol.h"
#include "ID.h"
#include "Leader/EU4Leader.h"
#include "Relations/EU4RelationDetails.h"
#include "Provinces/EU4Province.h"

namespace EU4
{
class Province;
class Version;
}
class EU4Loan;
class EU4Relations;

namespace mappers
{
class IdeaEffectMapper;
class TechgroupsMapper;
}



namespace EU4
{
	class Country: public commonItems::parser
	{
		public:
			Country() = default;
			Country(
				const std::string& countryTag,
				const EU4::Version& theVersion,
				std::istream& theStream,
				const mappers::IdeaEffectMapper& ideaEffectMapper
			);

			// Add any additional information available from the specified country file.
			void readFromCommonCountry(const std::string& fileName, const std::string& fullFilename);

			void setLocalisationName(const std::string& language, const std::string& name);
			void setLocalisationAdjective(const std::string& language, const std::string& adjective);

			void addProvince(Province&);
			void addCore(Province&);
	      void setInHRE(bool _inHRE) { inHRE = _inHRE; }
			void setEmperor(bool _emperor) { holyRomanEmperor = _emperor; }
			void setCelestialEmperor(bool _celestialEmperor) { celestialEmperor = _celestialEmperor; }
			bool hasModifier(std::string) const;
			int hasNationalIdea(std::string) const;
			bool hasFlag(std::string) const ;
			void resolveRegimentTypes(const mappers::UnitTypeMapper& utm);
			int getManufactoryCount() const;
			int numEmbracedInstitutions() const;
			void eatCountry(std::shared_ptr<Country> target, std::shared_ptr<Country> self);
			void setColonialRegion(const std::string& region) { colonialRegion = region; }
			void takeArmies(std::shared_ptr<Country>);
			void clearArmies();
			const void viveLaRevolution(bool revolting) { revolutionary = revolting; }

			bool cultureSurvivesInCores(const std::map<std::string, std::shared_ptr<EU4::Country>>& theCountries);

			std::string getTag() const { return tag; }
			void setTag(std::string _tag) { tag = _tag; }
			std::vector<Province>& getProvinces() { return provinces; }
			std::vector<Province>& getCores() { return cores; }
			int getCapital() const { return capital; }
			bool getInHRE() const { return inHRE; }
			bool getHolyRomanEmperor() const { return holyRomanEmperor; }
			bool getCelestialEmperor() const { return celestialEmperor; }
			std::string getTechGroup() const { return techGroup; }
			std::vector<bool> getEmbracedInstitutions() const { return embracedInstitutions; }
			int getIsolationism() const { return isolationism; }
			std::string getPrimaryCulture() const { return primaryCulture; }
			std::vector<std::string> getAcceptedCultures() const { return acceptedCultures; }
			std::optional<EU4::cultureGroup> getCulturalUnion() const { return culturalUnion; }
			std::string getReligion() const { return religion; }
			double getScore() const { return score; }
			double getStability() const { return stability; }
			virtual double getAdmTech() const { return admTech; }
			virtual double getDipTech() const { return dipTech; }
			virtual double getMilTech() const { return milTech; }
			double getArmyInvestment() const { return armyInvestment; }
			double getNavyInvestment() const { return navyInvestment; }
			double getCommerceInvestment() const { return commerceInvestment; }
			double getIndustryInvestment() const { return industryInvestment; }
			double getCultureInvestment() const { return cultureInvestment; }
			double getSlaveryInvestment() const { return slaveryInvestment; }
			double getUpper_house_compositionInvestment() const { return upper_house_compositionInvestment; }
			double getVote_franchiseInvestment() const { return vote_franchiseInvestment; }
			double getVoting_systemInvestment() const { return voting_systemInvestment; }
			double getPublic_meetingsInvestment() const { return public_meetingsInvestment; }
			double getPress_rightsInvestment() const { return press_rightsInvestment; }
			double getTrade_unionsInvestment() const { return trade_unionsInvestment; }
			double getPolitical_partiesInvestment() const { return political_partiesInvestment; }
			double getLibertyInvestment() const { return libertyInvestment; }
			double getEqualityInvestment() const { return equalityInvestment; }
			double getOrderInvestment() const { return orderInvestment; }
			double getLiteracyInvestment() const { return literacyInvestment; }
			double getReactionaryInvestment() const { return reactionaryInvestment; }
			double getLiberalInvestment() const { return liberalInvestment; }
			bool getPossibleDaimyo() const { return possibleDaimyo; }
			bool getPossibleShogun() const { return possibleShogun; }
			std::string getGovernment() const { return government; }
			std::set<std::string> getReforms() const { return governmentReforms; }
			std::map<std::string, EU4RelationDetails> getRelations() const { return relations; }
			std::vector<EU4Army> getArmies() const { return armies; }
			bool isCustom() const { return customNation; }
			bool isColony() const { return colony; }
			std::string getColonialRegion() const { return colonialRegion; }
			double getLibertyDesire() const { return libertyDesire; }
			bool isRevolutionary() const { return revolutionary; }
			std::string getRandomName() const { return randomName; }
			virtual const std::map<std::string, int>& getNationalIdeas() const { return nationalIdeas; }
			std::vector<EU4::Leader> getHistoricalLeaders() const { return historicalLeaders; }
			std::vector<EU4::Leader> getMilitaryLeaders() const { return militaryLeaders; }

			std::string	getName() const { return name; }
			std::string	getName(const std::string& language) const;
			std::string	getAdjective(const std::string& language) const;
			void dropMinorityCultures();
			void filterLeaders();

			EU4::NationalSymbol getNationalColors() const { return nationalColors; }

		private:
			void determineJapaneseRelations();
			void determineInvestments(const mappers::IdeaEffectMapper& ideaEffectMapper);
			void determineLibertyDesire();
			void determineCulturalUnion();
			void clearProvinces();
			void clearCores();

			std::string tag; // the tag for the EU4 nation
			std::vector<Province> provinces;
			std::vector<Province> cores;
			bool inHRE; // if this country is an HRE member
			bool holyRomanEmperor; // if this country is the emperor of the HRE
			bool celestialEmperor; // if this country is the celestial emperor
			int capital; // the EU4 province that is this nation's capital
			std::string techGroup; // the tech group for this nation
			std::vector<bool> embracedInstitutions; // the institutions this nation has embraced
			int isolationism; // the isolationism of the country (for Shinto nations with Mandate of Heaven)
			std::string primaryCulture; // the primary EU4 culture of this nation
			std::vector<std::string> acceptedCultures; // the accepted EU4 cultures for this nation
			std::optional<EU4::cultureGroup> culturalUnion;
			std::string religion; // the accepted religion of this country
			double score = 0.0;
			double stability; // the stability of this nation
			double admTech; // the admin tech of this nation
			double dipTech; // the diplo tech of this nation
			double milTech; // the mil tech of this nation

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


			std::map<std::string, bool> flags; // any flags set for this country
			std::map<std::string, bool> modifiers; // any modifiers set for this country
			bool possibleDaimyo = false; // if this country is possibly a daimyo
			bool possibleShogun = false; // if this country is the shogun
			std::vector<EU4::Leader> historicalLeaders; // Historical leader information
			std::set<int> activeLeaderIDs; // Ones currently in service, others presumed dead.
			std::vector<EU4::Leader> militaryLeaders; // filtered active leaders
			std::string government = "monarchy";
			int governmentRank = 0;
			int development = 0;
			std::map<std::string, EU4RelationDetails> relations; // the relations with other nations
			std::vector<EU4Army> armies; // both armies and navies
			std::map<std::string, int> nationalIdeas; // the national ideas for this country
			double legitimacy; // the legitimacy of this nation
			bool customNation; // whether or not this is a custom or random nation
			bool colony; // whether or not this country is a colony
			std::string overlord;
			std::string colonialRegion; // the colonial region, if this country is a colony
			double libertyDesire; // the amount of liberty desire
			std::string randomName; // the new name of this nation in Random World
			bool revolutionary; // does this country wave the glorious tricoloured banner of the revolution
			std::set<std::string> governmentReforms;

			// Localisation attributes
			std::string	name; // the name of this country
			std::string	adjective; // the adjective for this country

			// Colors
			EU4::NationalSymbol nationalColors;

			std::map<std::string, std::string> namesByLanguage; // the names of this country in different localisations
			std::map<std::string, std::string> adjectivesByLanguage;	// the adjectives for this country in different localisations
			std::map<std::string, double> states; // Areas which have been made states by this country.
	};
}



#endif // EU4_COUNTRY_H_
