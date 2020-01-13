#ifndef V2COUNTRY_H_
#define V2COUNTRY_H_



#include "V2Army.h"
#include "Country/V2Party.h"
#include "Leader/V2Leader.h"
#include "Leader/V2LeaderTraitMapper.h"
#include "V2Localisation.h"
#include "V2Relations.h"
#include "V2TechSchools.h"
#include "Color.h"
#include "Date.h"
#include "../EU4World/Army/EU4Army.h"
#include "../EU4World/Regions/Regions.h"
#include "../EU4World/Country/EU4NationalSymbol.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/GovernmentMapper.h"
#include "../Mappers/ReligionMapper.h"
#include "../Mappers/CountryMappings/CountryMappings.h"
#include "../Mappers/Adjacency/AdjacencyMapper.h"
#include "Country/V2Unreleasables.h"
#include <memory>
#include <set>
#include <vector>



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
class V2World;
class V2State;
class V2Province;
class V2Reforms;
class V2UncivReforms;
class V2Factory;
class V2Creditor;
class V2LeaderTraits;

enum class addRegimentToArmyResult
{
	success,
	retry,
	doNotRetry
};



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
			const std::unique_ptr<Vic2::TechSchools>& techSchools,
			const mappers::CultureMapper& cultureMapper,
			const mappers::CultureMapper& slaveCultureMapper,
			const mappers::IdeaEffectMapper& ideaEffectMapper,
			const mappers::ReligionMapper& religionMapper,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::GovernmentMapper& governmentMapper,
			const mappers::CountryMappings& countryMapper
		);
		void initFromHistory(const mappers::V2Unreleasables& unreleasablesMapper);
		void								addProvince(V2Province* _province);
		void								addState(V2State* newState);
		void convertArmies(
			double cost_per_regiment[static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories)],
			const std::map<int, V2Province*>& allProvinces,
			std::vector<int> port_whitelist,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::AdjacencyMapper& adjacencyMapper
		);
		void convertLeaders(mappers::V2LeaderTraitMapper& leaderTraits);
		bool addFactory(const V2Factory& factory);
		void								addRailroadtoCapitalState();
		void								convertUncivReforms(int techGroupAlgorithm, double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		void								oldCivConversionMethod();
		void								newCivConversionMethod(double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper);
		void								convertLandlessReforms(V2Country* capOwner);
		void setupPops(
			double popWeightRatio,
			int popConversionAlgorithm,
			const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
			const mappers::ProvinceMapper& provinceMapper
		);
		void								setArmyTech(double normalizedScore);
		void								setNavyTech(double normalizedScore);
		void								setCommerceTech(double normalizedScore);
		void								setIndustryTech(double normalizedScore);
		void								setCultureTech(double normalizedScore);
		void addRelation(V2Relations& newRelation);
		void								absorbVassal(V2Country* vassal);
		void								setColonyOverlord(V2Country* colony);
		V2Country*						getColonyOverlord();
		std::string							getColonialRegion();

		std::string							getLocalName();
		std::optional<V2Relations> getRelations(std::string withWhom) const;
		std::tuple<double, double, double> getNationalValueScores();
		
		void addPrestige(double additionalPrestige) { prestige += additionalPrestige; }
		double getPrestige() const { return prestige; }
		void								addResearchPoints(double newPoints)		{ researchPoints += newPoints; }
		void								addTech(std::string newTech)						{ techs.push_back(newTech); }
		void								setNationalValue(std::string NV)				{ nationalValue = NV; }
		void								isANewCountry(void)							{ newCountry = true; }

		virtual std::map<int, V2Province*> getProvinces() const { return provinces; }
		std::vector<V2State*>				getStates() const { return states; }
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
		V2UncivReforms*					getUncivReforms() const { return uncivReforms; }
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
		void determineTechSchool(const std::unique_ptr<Vic2::TechSchools>& techschools);
		void calculateLiteracy(std::shared_ptr<EU4::Country> srcCountry);
		void generateRelations(std::shared_ptr<EU4::Country> srcCountry, const mappers::CountryMappings& countryMapper);
		void resolvePolitics();
		void finalizeInvestments(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper);
		void determineGovernmentType(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::GovernmentMapper& governmentMapper);
		void setPrimaryAndAcceptedCultures(std::shared_ptr<EU4::Country> srcCountry, const mappers::CultureMapper& cultureMapper, const EU4::Regions& eu4Regions);
		void setReligion(std::shared_ptr<EU4::Country> srcCountry, const mappers::ReligionMapper& religionMapper);
		void loadPartiesFromBlob();

	private:
		void			outputTech(FILE*) const ;
		void			outputElection(FILE*) const;
		void			addLoan(std::string creditor, double size, double interest);
		addRegimentToArmyResult addRegimentToArmy(
			V2Army& army,
			EU4::REGIMENTCATEGORY rc,
			std::map<int, V2Province*> allProvinces,
			const mappers::ProvinceMapper& provinceMapper,
			const mappers::AdjacencyMapper& adjacencyMapper
		);
		std::vector<int> getPortProvinces(const std::vector<int>& locationCandidates, std::map<int, V2Province*> allProvinces);
		V2Army* getArmyForRemainder(EU4::REGIMENTCATEGORY rc);
		V2Province*	getProvinceForExpeditionaryArmy();
		std::string		getRegimentName(EU4::REGIMENTCATEGORY rc);

		const V2World* theWorld;
		std::shared_ptr<EU4::Country> srcCountry;
		std::string							filename;
		bool								newCountry;			// true if this country is being added by the converter, i.e. doesn't already exist in Vic2
		bool								dynamicCountry;	// true if this country is a Vic2 dynamic country
		V2Country*						colonyOverlord;
		std::string							colonialRegion;

		std::string							tag;
		std::vector<V2State*>				states;
		std::map<int, V2Province*>		provinces;
		int capital = 0;
		bool								civilized;
		bool isReleasableVassal = false;
		bool								holyRomanEmperor;
		bool								inHRE;
		bool								celestialEmperor;
		std::string							primaryCulture;
		std::set<std::string>						acceptedCultures;
		std::string							religion;
		std::vector<V2Party> parties;
		std::string							rulingParty;
		std::string							commonCountryFile;
		double prestige = 0.0;
		double							leadership;
		double							plurality;
		std::vector<std::string>					techs;
		std::set<std::string>						inventions;
		V2UncivReforms*				uncivReforms;
		double							researchPoints;
		std::string							techSchool;
		std::string							government;
		int								upperHouseReactionary;
		int								upperHouseConservative;
		int								upperHouseLiberal;
		std::vector<std::pair<int, int>>	reactionaryIssues;
		std::vector<std::pair<int, int>>	conservativeIssues;
		std::vector<std::pair<int, int>>	liberalIssues;
		std::map<std::string, V2Relations>	relations;
		std::vector<V2Army> armies;
		V2Reforms*						reforms;
		std::string nationalValue = "nv_order";
		double							money;
		date								lastBankrupt;
		std::map<std::string, V2Creditor*>	creditors;
		double							bankReserves;
		double							diploPoints;
		double							badboy;
		std::vector<V2Leader> leaders;
		double							literacy;
		V2Localisation					localisation;
		EU4::NationalSymbol nationalColors;
		int								unitNameCount[static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories)];
		int								numFactories;
		std::vector<std::string>					decisions;

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

bool ProvinceRegimentCapacityPredicate(V2Province* prov1, V2Province* prov2);


#endif	// V2COUNTRY_H_