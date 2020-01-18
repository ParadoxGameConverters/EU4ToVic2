#ifndef V2COUNTRY_H_
#define V2COUNTRY_H_

#include "Party/Party.h"
#include "Leader/Leader.h"
#include "Localisation/Localisation.h"
#include "Diplomacy/Relation.h"
#include "Date.h"
#include "../EU4World/Regions/Regions.h"
#include "../EU4World/Country/EU4NationalSymbol.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/GovernmentMapper/GovernmentMapper.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Mappers/CountryMappings/CountryMappings.h"
#include "../Mappers/Adjacency/AdjacencyMapper.h"
#include "../Mappers/PartyNames/PartyNameMapper.h"
#include "../Mappers/PartyTypes/PartyTypeMapper.h"
#include "../Mappers/TechSchools/TechSchoolMapper.h"
#include "../Mappers/Unreleasables/Unreleasables.h"
#include "../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "../Mappers/PortProvinces/PortProvinces.h"
#include "../Mappers/RegimentCosts/RegimentCostsMapper.h"
#include <memory>
#include <set>
#include <vector>
#include "Factory/Factory.h"
#include "Army/Regiment.h"
#include "Army/Army.h"
#include "Province/Province.h"

namespace EU4
{
class Country;
class World;
}
namespace mappers
{
class IdeaEffectMapper;
class TechGroupsMapper;
}
namespace V2
{
	class Reforms;
	class UncivReforms;
	class V2World;
	class State;
}

class V2Creditor;
class V2LeaderTraits;


namespace V2{

class V2Country : commonItems::parser
{
	public:
		V2Country(const std::string& countriesFileLine, const V2World* _theWorld, bool _dynamicCountry);
		V2Country(const std::string& _tag, const std::string& _commonCountryFile, const V2World* _theWorld);
		V2Country() = default;

		void								output() const;
		void								outputToCommonCountriesFile(FILE*) const;
		void outputLocalisation(std::ostream& output) const;
		void								outputOOB() const;
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
		void								addProvince(std::shared_ptr<V2::Province> _province);
		void								addState(State* newState, const mappers::PortProvinces& portProvincesMapper);
		void convertArmies(
			const mappers::RegimentCostsMapper& regimentCostsMapper,
			const std::map<int, std::shared_ptr<V2::Province>>& allProvinces,
			const mappers::PortProvinces& portProvincesMapper,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::AdjacencyMapper& adjacencyMapper
		);
		void convertLeaders(mappers::LeaderTraitMapper& leaderTraitMapper);
		bool addFactory(std::shared_ptr<Factory> factory);
		void								addRailroadtoCapitalState();
		void								convertUncivReforms(int techGroupAlgorithm, double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		void								oldCivConversionMethod();
		void								newCivConversionMethod(double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		void								convertLandlessReforms(V2Country* capOwner);
		void setupPops(
			double popWeightRatio,
			int popConversionAlgorithm,
			const mappers::ProvinceMapper& provinceMapper
		);
		void								setArmyTech(double normalizedScore);
		void								setNavyTech(double normalizedScore);
		void								setCommerceTech(double normalizedScore);
		void								setIndustryTech(double normalizedScore);
		void								setCultureTech(double normalizedScore);
		void addRelation(V2::Relation& newRelation);
		void								absorbVassal(V2Country* vassal);
		void								setColonyOverlord(V2Country* colony);
		V2Country*						getColonyOverlord();
		std::string							getColonialRegion();

		std::string							getLocalName();
		V2::Relation& getRelation(const std::string& withWhom);
		std::tuple<double, double, double> getNationalValueScores();
		
		void addPrestige(double additionalPrestige) { prestige += additionalPrestige; }
		double getPrestige() const { return prestige; }
		void								addResearchPoints(double newPoints)		{ researchPoints += newPoints; }
		void								addTech(std::string newTech)						{ techs.push_back(newTech); }
		void								setNationalValue(std::string NV)				{ nationalValue = NV; }
		void								isANewCountry(void)							{ newCountry = true; }

		const std::map<int, std::shared_ptr<V2::Province>>& getProvinces() const { return provinces; }
		std::vector<State*>				getStates() const { return states; }
		std::string							getTag() const { return tag; }
		virtual bool isCivilized() const { return civilized; }
		std::string							getPrimaryCulture() const { return primaryCulture; }
		std::set<std::string>						getAcceptedCultures() const { return acceptedCultures; }
		virtual std::shared_ptr<EU4::Country> getSourceCountry() const { return srcCountry; }
		double							getReactionary() const { return upperHouseReactionary; }
		double							getConservative() const { return upperHouseConservative; }
		double							getLiberal() const { return upperHouseLiberal; }
		std::string							getGovernment() const { return government; }
		std::vector<std::pair<int, int>>	getReactionaryIssues() const { return reactionaryIssues; }
		std::vector<std::pair<int, int>>	getConservativeIssues() const { return conservativeIssues; }
		std::vector<std::pair<int, int>>	getLiberalIssues() const { return liberalIssues; }
		double							getLiteracy() const { return literacy; }
		V2::UncivReforms* getUncivReforms() const { return uncivReforms; }
		int								getCapital() const { return capital; }
		bool								isNewCountry() const { return newCountry; }
		int								getNumFactories() const { return numFactories; }

		std::string							getReligion() const { return religion; }

		double getSlavery() const { return slaveryInvestment; }
		double getUpper_house_composition() const { return upper_house_compositionInvestment; }
		double getVote_franchise() const { return vote_franchiseInvestment; }
		double getVoting_system() const { return voting_systemInvestment; }
		double getPublic_meetings() const { return public_meetingsInvestment; }
		double getPress_rights() const { return press_rightsInvestment; }
		double getTrade_unions() const { return trade_unionsInvestment; }
		double getPolitical_parties() const { return political_partiesInvestment; }

		void buildCanals(std::shared_ptr<EU4::Country> srcCountry);
		void determineTechSchool(const mappers::TechSchoolMapper& techSchoolMapper);
		void calculateLiteracy(std::shared_ptr<EU4::Country> srcCountry);
		void generateRelations(std::shared_ptr<EU4::Country> srcCountry, const mappers::CountryMappings& countryMapper);
		void resolvePolitics();
		void finalizeInvestments(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void determineGovernmentType(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::GovernmentMapper& governmentMapper);
		void setPrimaryAndAcceptedCultures(std::shared_ptr<EU4::Country> srcCountry, const mappers::CultureMapper& cultureMapper, const EU4::Regions& eu4Regions);
		void setReligion(std::shared_ptr<EU4::Country> srcCountry, const mappers::ReligionMapper& religionMapper);
		void loadPartiesFromBlob(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper);

	private:
		void outputTech(std::ofstream& output) const ;
		void			outputElection(FILE*) const;
		void			addLoan(std::string creditor, double size, double interest);
		V2::Army* getArmyForRemainder(V2::REGIMENTTYPE chosenType);
	
		const V2World* theWorld;
		std::shared_ptr<EU4::Country> srcCountry;
		std::string							filename;
		bool								newCountry;			// true if this country is being added by the converter, i.e. doesn't already exist in Vic2
		bool								dynamicCountry;	// true if this country is a Vic2 dynamic country
		V2Country*						colonyOverlord;
		std::string							colonialRegion;

		std::string							tag;
		std::vector<State*>				states;
		std::map<int, std::shared_ptr<V2::Province>> provinces;
		int capital = 0;
		bool civilized;
		bool isReleasableVassal = false;
		bool								holyRomanEmperor;
		bool								inHRE;
		bool								celestialEmperor;
		std::string							primaryCulture;
		std::set<std::string>						acceptedCultures;
		std::string							religion;
		std::vector<V2::Party> parties;
		std::string							rulingParty;
		std::string							commonCountryFile;
		double prestige = 0.0;
		double							leadership;
		double							plurality;
		std::vector<std::string>					techs;
		std::set<std::string>						inventions;
		V2::UncivReforms* uncivReforms;
		double							researchPoints;
		std::string							techSchool;
		std::string							government;
		int								upperHouseReactionary;
		int								upperHouseConservative;
		int								upperHouseLiberal;
		std::vector<std::pair<int, int>>	reactionaryIssues;
		std::vector<std::pair<int, int>>	conservativeIssues;
		std::vector<std::pair<int, int>>	liberalIssues;
		std::map<std::string, V2::Relation>	relations;
		std::vector<V2::Army> armies;
		V2::Reforms* reforms;
		std::string nationalValue = "nv_order";
		double							money;
		date								lastBankrupt;
		std::map<std::string, V2Creditor*>	creditors;
		double							bankReserves;
		double							diploPoints;
		double							badboy;
		std::vector<V2::Leader> leaders;
		double							literacy;
		V2::Localisation localisation;
		EU4::NationalSymbol nationalColors;
		std::map<V2::REGIMENTTYPE, int> unitNameCount;
		int								numFactories;
		std::vector<std::string>					decisions;
		std::map<V2::REGIMENTTYPE, double> countryRemainders;

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


};
}


#endif	// V2COUNTRY_H_