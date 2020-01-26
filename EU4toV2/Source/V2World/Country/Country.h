#ifndef COUNTRY_H
#define COUNTRY_H

#include <memory>
#include <string>
#include "CountryDetails.h"
#include "../Localisation/Localisation.h"
#include "../Reforms/UncivReforms.h"
#include "../Reforms/Reforms.h"
#include "../Province/Province.h"
#include "../State/State.h"
#include "../Leader/Leader.h"
#include "../Army/Army.h"
#include "../Diplomacy/Relation.h"

namespace EU4
{
	class Country;
	class World;
	class Regions;
}
namespace mappers
{
	class RegimentCostsMapper;
	class AdjacencyMapper;
	class LeaderTraitMapper;
	class PortProvinces;
	class Unreleasables;
	class TechSchoolMapper;
	class CultureMapper;
	class ReligionMapper;
	class GovernmentMapper;
	class CountryMappings;
	class PartyNameMapper;
	class PartyTypeMapper;
	class IdeaEffectMapper;
	class TechGroupsMapper;
}
namespace V2
{
	enum class CIV_ALGORITHM { older, newer };

	class Relation;
	class Country
	{
	public:
		Country() = default;
		Country(
			const std::string& countriesFileLine, 
			bool _dynamicCountry, 
			const mappers::PartyNameMapper& partyNameMapper, 
			const mappers::PartyTypeMapper& partyTypeMapper);
		Country(
			std::string _tag, 
			const std::string& _commonCountryFile, 
			const mappers::PartyNameMapper& partyNameMapper,
			const mappers::PartyTypeMapper& partyTypeMapper);
		
		void initFromEU4Country(
			const EU4::Regions& eu4Regions,
			std::shared_ptr<EU4::Country> _srcCountry,
			const mappers::TechSchoolMapper& techSchoolMapper,
			const mappers::CultureMapper& cultureMapper,
			const mappers::IdeaEffectMapper& ideaEffectMapper,
			const mappers::ReligionMapper& religionMapper,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::GovernmentMapper& governmentMapper,
			const mappers::CountryMappings& countryMapper
		);
		void initFromHistory(const mappers::Unreleasables& unreleasablesMapper);
		void setupPops(
			double popWeightRatio,
			CIV_ALGORITHM popConversionAlgorithm,
			const mappers::ProvinceMapper& provinceMapper
		);
		void convertArmies(
			const mappers::RegimentCostsMapper& regimentCostsMapper,
			const std::map<int, std::shared_ptr<V2::Province>>& allProvinces,
			const mappers::PortProvinces& portProvincesMapper,
			const mappers::ProvinceMapper& provinceMapper
		);
		
		void setNewCountry() { newCountry = true; }
		void setColonyOverlord(const std::string& colony) { colonyOverlord = colony; }
		void addRelation(Relation& newRelation);
		void addPrestige(const double additionalPrestige) { details.prestige += additionalPrestige; }
		void setPrestige(const double prestige) { details.prestige = prestige; }
		void absorbColony(Country& vassal);
		void addProvince(std::shared_ptr<Province> _province);
		void addState(std::shared_ptr<State> newState, const mappers::PortProvinces& portProvincesMapper);
		void convertLeaders(const mappers::LeaderTraitMapper& leaderTraitMapper);
		void convertUncivReforms(CIV_ALGORITHM techGroupAlgorithm, double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		void addRailroadtoCapitalState();
		void addTech(const std::string& newTech) { techs.insert(newTech); }
		void convertLandlessReforms(const Country& capitalOwner);
		void setArmyTech(double normalizedScore);
		void setNavyTech(double normalizedScore);
		void setCommerceTech(double normalizedScore);
		void setIndustryTech(double normalizedScore);
		void setCultureTech(double normalizedScore);
		void setNationalValue(const std::string& nv) { details.nationalValue = nv; }

		bool addFactory(std::shared_ptr<Factory> factory);
		Relation& getRelation(const std::string& target);
		std::map<std::string, Relation>& getRelations() { return relations; }

		[[nodiscard]] std::string getColonialRegion() const;
		[[nodiscard]] std::shared_ptr<EU4::Country> getSourceCountry() const { return srcCountry; }
		[[nodiscard]] std::optional<UncivReforms> getUncivReforms() const { return uncivReforms; }
		[[nodiscard]] NationalValue getNationalValueScores() const;
		[[nodiscard]] auto isNewCountry() const { return newCountry; }
		[[nodiscard]] auto getNumFactories() const { return details.numFactories; }
		[[nodiscard]] auto getPrestige() const { return details.prestige; }
		[[nodiscard]] auto isDynamicCountry() const { return dynamicCountry; }
		[[nodiscard]] auto isCivilized() const { return details.civilized; }
		[[nodiscard]] auto getCapital() const { return details.capital; }
		[[nodiscard]] auto getLocalName() const { return localisation.getLocalName(); }
		[[nodiscard]] const auto& getStates() const { return states; }
		[[nodiscard]] const auto& getTag() const { return tag; }
		[[nodiscard]] const auto& getColonyOverlord() const { return colonyOverlord; }
		[[nodiscard]] const auto& getProvinces() const { return provinces; }
		[[nodiscard]] const auto& getCommonCountryFile() const { return commonCountryFile; }
		[[nodiscard]] const auto& getReligion() const { return details.religion; }
		[[nodiscard]] const auto& getLocalisation() const { return localisation; }
		[[nodiscard]] const auto& getFilename() const { return details.filename; }

		friend std::ostream& operator<<(std::ostream& output, const Country& country);
		void outputCommons(std::ostream& output);
		void outputOOB(std::ostream& output);

	private:
		bool dynamicCountry = false;	// true if this country is a Vic2 dynamic country
		bool newCountry = false; // true if this country is being added by the converter, i.e. doesn't already exist in Vic2
		std::string tag;
		std::string commonCountryFile;
		std::shared_ptr<EU4::Country> srcCountry;
		std::optional<UncivReforms> uncivReforms;
		std::optional<Reforms> reforms;
		std::string colonyOverlord;
		std::map<std::string, Relation> relations;
		std::map<int, std::shared_ptr<Province>> provinces;
		std::set<std::string> decisions;
		std::vector<std::shared_ptr<State>> states;
		std::vector<Leader> leaders;
		std::set<std::string> techs;
		std::set<std::string> inventions;
		std::map<REGIMENTTYPE, int> unitNameCount;
		std::map<REGIMENTTYPE, double> countryRemainders;
		std::vector<Army> armies;
		CountryDetails details;
		Localisation localisation;
		EU4::NationalSymbol nationalColors;

		void loadPartiesFromBlob(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper);
		void initParties(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper);
		void setReligion(const std::string& religion, const mappers::ReligionMapper& religionMapper);
		void setPrimaryAndAcceptedCultures(const mappers::CultureMapper& cultureMapper, const EU4::Regions& eu4Regions);
		void determineGovernmentType(const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::GovernmentMapper& governmentMapper);
		void finalizeInvestments(const mappers::IdeaEffectMapper& ideaEffectMapper);
		void resolvePolitics();
		void generateRelations(const mappers::CountryMappings& countryMapper);
		void calculateLiteracy();
		void calculateConsciousness();
		void determineTechSchool(const mappers::TechSchoolMapper& techSchoolMapper);
		void buildCanals();
		void oldCivConversionMethod();
		void newCivConversionMethod(double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		Army* getArmyForRemainder(REGIMENTTYPE chosenType);
	};
}

#endif // COUNTRY_H