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
			const std::string& _tag, 
			const std::string& _commonCountryFile, 
			const mappers::PartyNameMapper& partyNameMapper,
			const mappers::PartyTypeMapper& partyTypeMapper);
		void initFromEU4Country(
			const EU4::Regions& eu4Regions,
			std::shared_ptr<EU4::Country> _srcCountry,
			const mappers::TechSchoolMapper& techSchoolMapper,
			const mappers::CultureMapper& cultureMapper,
			const mappers::CultureMapper& slaveCultureMapper,
			const mappers::IdeaEffectMapper& ideaEffectMapper,
			const mappers::ReligionMapper& religionMapper,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::GovernmentMapper& governmentMapper,
			const mappers::CountryMappings& countryMapper
		);
		void initFromHistory(const mappers::Unreleasables& unreleasablesMapper);
		void setupPops(
			double popWeightRatio,
			int popConversionAlgorithm,
			const mappers::ProvinceMapper& provinceMapper
		);
		void convertArmies(
			const mappers::RegimentCostsMapper& regimentCostsMapper,
			const std::map<int, std::shared_ptr<V2::Province>>& allProvinces,
			const mappers::PortProvinces& portProvincesMapper,
			const mappers::ProvinceMapper& provinceMapper
		);
		
		void setNewCountry() { newCountry = true; }
		bool isNewCountry() const { return newCountry; }
		const std::string& getTag() const { return tag; }
		std::shared_ptr<EU4::Country> getSourceCountry() const { return srcCountry; }
		const std::string& getColonyOverlord() const { return colonyOverlord; }
		void setColonyOverlord(const std::string& colony) { colonyOverlord = colony; }
		int getNumFactories() const { return details.numFactories; }
		Relation& getRelation(const std::string& withWhom);
		void addRelation(Relation& newRelation);
		void absorbColony(Country& vassal);
		const std::map<int, std::shared_ptr<Province>>& getProvinces() const { return provinces; }
		void addPrestige(double additionalPrestige) { details.prestige += additionalPrestige; }
		double getPrestige() const { return details.prestige; }
		const NationalValueInvestments& getNationalValueScores() const;
		void setNationalValue(const std::string& NV) { details.nationalValue = NV; }
		std::vector<std::shared_ptr<State>> getStates() const { return states; }
		bool isCivilized() const { return details.civilized; }
		void addProvince(std::shared_ptr<Province> _province);
		int getCapital() const { return details.capital; }
		void addState(std::shared_ptr<State> newState, const mappers::PortProvinces& portProvincesMapper);
		void convertLeaders(const mappers::LeaderTraitMapper& leaderTraitMapper);
		void convertUncivReforms(int techGroupAlgorithm, double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		void addRailroadtoCapitalState();
		void addTech(const std::string& newTech) { techs.insert(newTech); }
		void convertLandlessReforms(std::shared_ptr<Country> capitalOwner);
		std::optional<UncivReforms> getUncivReforms() const { return uncivReforms; }
		void setArmyTech(double normalizedScore);
		void setNavyTech(double normalizedScore);
		void setCommerceTech(double normalizedScore);
		void setIndustryTech(double normalizedScore);
		void setCultureTech(double normalizedScore);
		bool addFactory(std::shared_ptr<Factory> factory);
		const std::string& getCommonCountryFile() const { return commonCountryFile; }
		std::string getLocalName() const { return localisation.getLocalName(); };
		const std::string& getReligion() const { return details.religion; }
		std::string getColonialRegion();
		const Localisation& getLocalisation() const { return localisation; }
		bool isDynamicCountry() const { return dynamicCountry; }
		const std::string& getFilename() const { return details.filename; }

		friend std::ostream& operator<<(std::ostream& output, const Country& country);
		void outputCommons(std::ostream& output);
		void outputOOB(std::ostream& output);
		
	private:
		bool dynamicCountry = false;	// true if this country is a Vic2 dynamic country
		bool newCountry = false; // true if this country is being added by the converter, i.e. doesn't already exist in Vic2
		CountryDetails details;
		std::string tag;
		std::string commonCountryFile;
		Localisation localisation;
		std::shared_ptr<EU4::Country> srcCountry;
		std::optional<UncivReforms> uncivReforms;
		std::optional<Reforms> reforms;
		std::string colonyOverlord;
		std::map<std::string, Relation> relations;
		std::map<int, std::shared_ptr<Province>> provinces;
		std::set<std::string> decisions;
		EU4::NationalSymbol nationalColors;
		std::vector<std::shared_ptr<State>> states;
		std::vector<Leader> leaders;
		std::set<std::string> techs;
		std::set<std::string> inventions;
		std::map<REGIMENTTYPE, int> unitNameCount;
		std::map<REGIMENTTYPE, double> countryRemainders;
		std::vector<Army> armies;

		void loadPartiesFromBlob(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper);
		void initParties(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper);
		void setReligion(const std::string& religion, const mappers::ReligionMapper& religionMapper);
		void setPrimaryAndAcceptedCultures(std::shared_ptr<EU4::Country> srcCountry, const mappers::CultureMapper& cultureMapper, const EU4::Regions& eu4Regions);
		void determineGovernmentType(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::GovernmentMapper& governmentMapper);
		void finalizeInvestments(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void resolvePolitics();
		void generateRelations(std::shared_ptr<EU4::Country> srcCountry, const mappers::CountryMappings& countryMapper);
		void calculateLiteracy(std::shared_ptr<EU4::Country> srcCountry);
		void determineTechSchool(const mappers::TechSchoolMapper& techSchoolMapper);
		void buildCanals(std::shared_ptr<EU4::Country> srcCountry);
		void oldCivConversionMethod();
		void newCivConversionMethod(double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		Army* getArmyForRemainder(REGIMENTTYPE chosenType);
	};
}

#endif // COUNTRY_H