#include "OSCompatibilityLayer.h"
#include "Country.h"
#include "Log.h"
#include "../../Mappers/PartyNames/PartyNameMapper.h"
#include "../../Mappers/PartyTypes/PartyTypeMapper.h"
#include "../../Mappers/ReligionMapper/ReligionMapper.h"
#include "../../Mappers/CultureMapper/CultureMapper.h"
#include "../../Mappers/GovernmentMapper/GovernmentMapper.h"
#include "../../Mappers/CountryMappings/CountryMappings.h"
#include "../../Mappers/TechSchools/TechSchoolMapper.h"
#include "../../Mappers/Unreleasables/Unreleasables.h"
#include "../../Mappers/LeaderTraits/LeaderTraitMapper.h"
#include "../../Mappers/TechGroups/TechGroupsMapper.h"
#include "../../Mappers/RegimentCosts/RegimentCostsMapper.h"
#include "../Flags/Flags.h"
#include <cmath>

V2::Country::Country(
	const std::string& countriesFileLine, 
	bool _dynamicCountry,
	const mappers::PartyNameMapper& partyNameMapper,
	const mappers::PartyTypeMapper& partyTypeMapper):
dynamicCountry(_dynamicCountry)
{
	// Load from a country file, if one exists. Otherwise rely on defaults.
	std::string filename;
	int start = countriesFileLine.find_first_of('/');
	start++;
	unsigned int size = countriesFileLine.find_last_of('\"') - start;
	filename = countriesFileLine.substr(start, size);
	details = mappers::CountryDetails(filename);
	tag = countriesFileLine.substr(0, 3);	
	commonCountryFile = localisation.convert(filename);
	initParties(partyNameMapper, partyTypeMapper);
}

V2::Country::Country(
	const std::string& _tag, 
	const std::string& _commonCountryFile, 
	const mappers::PartyNameMapper& partyNameMapper,
	const mappers::PartyTypeMapper& partyTypeMapper):
tag(_tag)
{
	newCountry = true;
	dynamicCountry = false;
	commonCountryFile = localisation.convert(_commonCountryFile);

	initParties(partyNameMapper, partyTypeMapper);
}

void V2::Country::initParties(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper)
{
	// We're a new nation so no parties are specified. Grab some.
	if (details.parties.empty()) loadPartiesFromBlob(partyNameMapper, partyTypeMapper);

	// set a default ruling party
	for (const auto& party : details.parties)
	{
		// We're pinging against this date only to protect against later-game parties.
		if (party.isActiveOn(date("1836.1.1")))
		{
			// This will get overridden if this country is initialized against extant EU4 country.
			details.rulingParty = party.getName();
			break;
		}
	}
}

void V2::Country::loadPartiesFromBlob(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper)
{
	size_t ideology = 0;
	for (const auto& partyName: partyNameMapper.getMap())
	{
		std::string partyKey = tag + '_' + partyName.first;
		auto languageMap = partyName.second.getMap();
		auto partyType = partyTypeMapper.getPartyTypeByIdeology(partyName.first);
		if (!partyType)
		{
			Log(LogLevel::Warning) << "Party type " << partyName.first << " has no entry in party_blobs.txt!";
			continue;
		}
		partyType->setName(partyKey);
		Party newParty(*partyType);
		details.parties.push_back(newParty);
		localisation.setPartyKey(ideology, partyKey);

		for (const auto& language : languageMap) localisation.setPartyName(ideology, language.first, language.second);

		++ideology;
	}
}

void V2::Country::initFromEU4Country(
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
) {
	srcCountry = _srcCountry;

	if (!srcCountry->getRandomName().empty()) newCountry = true;

	auto possibleFilename = Utils::GetFileFromTag("./blankMod/output/history/countries/", tag);
	if (!possibleFilename)
	{
		possibleFilename = Utils::GetFileFromTag(theConfiguration.getVic2Path() + "/history/countries/", tag);
	}

	if (!possibleFilename)
	{
		std::string countryName = commonCountryFile;
		int lastSlash = countryName.find_last_of("/");
		countryName = countryName.substr(lastSlash + 1, countryName.size());
		details.filename = tag + " - " + countryName;
	}
	else
	{
		details.filename = *possibleFilename;
	}

	// Color
	nationalColors = srcCountry->getNationalColors();

	// Localisation
	localisation.setTag(tag);
	localisation.readFromCountry(*srcCountry);

	// Capital
	int oldCapital = srcCountry->getCapital();
	auto potentialCapitals = provinceMapper.getVic2ProvinceNumbers(oldCapital);
	if (!potentialCapitals.empty()) details.capital = *potentialCapitals.begin();

	// HRE / Emperors
	details.inHRE = srcCountry->getInHRE();
	details.holyRomanEmperor = srcCountry->getHolyRomanEmperor();
	details.celestialEmperor = srcCountry->getCelestialEmperor();

	// religion
	setReligion(_srcCountry, religionMapper);

	// cultures
	setPrimaryAndAcceptedCultures(_srcCountry, cultureMapper, eu4Regions);

	// Government
	determineGovernmentType(_srcCountry, ideaEffectMapper, governmentMapper);

	// Apply government effects to reforms
	finalizeInvestments(_srcCountry, ideaEffectMapper);

	//  Politics
	resolvePolitics();

	// Generate Reforms
	reforms = Reforms(details, srcCountry);

	// Relations
	generateRelations(_srcCountry, countryMapper);

	// Literacy and Tech school
	calculateLiteracy(_srcCountry);
	determineTechSchool(techSchoolMapper);

	// Misc
	buildCanals(_srcCountry);
}

void V2::Country::setReligion(std::shared_ptr<EU4::Country> srcCountry, const mappers::ReligionMapper& religionMapper)
{
	std::string srcReligion = srcCountry->getReligion();
	std::optional<std::string> match = religionMapper.getVic2Religion(srcReligion);
	if (!match)
	{
		LOG(LogLevel::Warning) << "No religion mapping defined for " << srcReligion << " (" << srcCountry->getTag() << " -> " << tag << ')';
	}
	else
	{
		details.religion = *match;
	}
}

void V2::Country::setPrimaryAndAcceptedCultures(std::shared_ptr<EU4::Country> srcCountry, const mappers::CultureMapper& cultureMapper, const EU4::Regions& eu4Regions)
{
	int oldCapital = srcCountry->getCapital();

	// primary culture
	std::string primCulture = srcCountry->getPrimaryCulture();
	std::optional<std::string> matched = cultureMapper.cultureMatch(eu4Regions, primCulture, details.religion, oldCapital, srcCountry->getTag());
	if (!matched)
	{
		LOG(LogLevel::Warning) << "No culture mapping defined for " << primCulture << " (" << srcCountry->getTag() << " -> " << tag << ')';
	}
	else
	{
		details.primaryCulture = *matched;
	}

	//accepted cultures
	std::vector<std::string> srcAceptedCultures = srcCountry->getAcceptedCultures();
	auto culturalUnion = srcCountry->getCulturalUnion();
	if (culturalUnion)
	{
		for (auto unionCulture : culturalUnion->getCultures())
		{
			srcAceptedCultures.push_back(unionCulture.first);
		}
	}
	for (auto srcCulture : srcAceptedCultures)
	{
		std::optional<std::string> dstCulture;
		dstCulture = cultureMapper.cultureMatch(
			eu4Regions,
			srcCulture,
			 details.religion,
			oldCapital,
			srcCountry->getTag()
		);
		if (dstCulture)
		{
			if (details.primaryCulture != *dstCulture) details.acceptedCultures.insert(*dstCulture);
		}
		else
		{
			LOG(LogLevel::Warning) << "No culture mapping defined for " << srcCulture << " (" << srcCountry->getTag() << " -> " << tag << ')';
		}
	}
}

std::tuple<double, double, double> V2::Country::getNationalValueScores()
{
	double orderScore = 0.0;
	double libertyScore = 0.0;
	double equalityScore = 0.0;

	if (srcCountry)
	{
		orderScore += srcCountry->getOrderInvestment() - 5.0;
		libertyScore += srcCountry->getLibertyInvestment() - 5.0;
		equalityScore += srcCountry->getEqualityInvestment() - 5.0;
	}

	return std::make_tuple(libertyScore, equalityScore, orderScore);
}

void V2::Country::addRelation(Relation& newRelation)
{
	relations.insert(std::make_pair(newRelation.getTarget(), newRelation));
}

V2::Relation& V2::Country::getRelation(const std::string& target)
{
	const auto& relation = relations.find(target);
	if (relation != relations.end()) return relation->second;
	Relation newRelation(target);
	relations.insert(std::make_pair(target, newRelation));
	const auto& newRelRef = relations.find(target);
	return newRelRef->second;
}

void V2::Country::absorbVassal(std::shared_ptr<Country> vassal)
{
	// change province ownership and add owner cores if needed
	for (auto& province : vassal->getProvinces())
	{
		province.second->setOwner(tag);
		province.second->setController(tag);
		province.second->addCore(tag);
	}
	vassal->provinces.clear();

	// take vassal's armies
	srcCountry->takeArmies(vassal->getSourceCountry());

	// assume the vassal's decisions (just canals, at the moment)
	for (const auto& decision : vassal->decisions) decisions.insert(decision);
	vassal->decisions.clear();
}

void V2::Country::determineGovernmentType(
	std::shared_ptr<EU4::Country> srcCountry, 
	const mappers::IdeaEffectMapper& ideaEffectMapper, 
	const mappers::GovernmentMapper& governmentMapper)
{
	details.government = governmentMapper.matchGovernment(srcCountry->getGovernment());

	for (auto reformStr : srcCountry->getReforms())
	{
		auto enforce = ideaEffectMapper.getEnforceFromIdea(reformStr);
		if (!enforce.empty()) details.government = enforce;
	}

	// Almost but not quite. Hardcoded but hey.
	if (srcCountry->isRevolutionary())
	{
		details.government = "bourgeois_dictatorship";
	}
}

void V2::Country::finalizeInvestments(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	// Collect and finalize all idea/reform/government effects. We have combined reforms + ideas incoming, but lack government component (the last 33%)
	// Resulting scores for all of these will be between 0 and 10, with 5 being average and supposed to be ignored.
	// Each point above or below 5 should alter absolute values by 10%.

	details.armyInvestment = (2 * srcCountry->getArmyInvestment() + ideaEffectMapper.getArmyFromIdea(details.government)) / 3;
	details.navyInvestment = (2 * srcCountry->getNavyInvestment() + ideaEffectMapper.getNavyFromIdea(details.government)) / 3;
	details.commerceInvestment = (2 * srcCountry->getCommerceInvestment() + ideaEffectMapper.getCommerceFromIdea(details.government)) / 3;
	details.industryInvestment = (2 * srcCountry->getIndustryInvestment() + ideaEffectMapper.getIndustryFromIdea(details.government)) / 3;
	details.cultureInvestment = (2 * srcCountry->getCultureInvestment() + ideaEffectMapper.getCultureFromIdea(details.government)) / 3;
	details.slaveryInvestment = (2 * srcCountry->getSlaveryInvestment() + ideaEffectMapper.getSlaveryFromIdea(details.government)) / 3;
	details.upper_house_compositionInvestment = (2 * srcCountry->getUpper_house_compositionInvestment() + ideaEffectMapper.getUpper_house_compositionFromIdea(details.government)) / 3;
	details.vote_franchiseInvestment = (2 * srcCountry->getVote_franchiseInvestment() + ideaEffectMapper.getVote_franchiseFromIdea(details.government)) / 3;
	details.voting_systemInvestment = (2 * srcCountry->getVoting_systemInvestment() + ideaEffectMapper.getVoting_systemFromIdea(details.government)) / 3;
	details.public_meetingsInvestment = (2 * srcCountry->getPublic_meetingsInvestment() + ideaEffectMapper.getPublic_meetingsFromIdea(details.government)) / 3;
	details.press_rightsInvestment = (2 * srcCountry->getPress_rightsInvestment() + ideaEffectMapper.getPress_rightsFromIdea(details.government)) / 3;
	details.trade_unionsInvestment = (2 * srcCountry->getTrade_unionsInvestment() + ideaEffectMapper.getTrade_unionsFromIdea(details.government)) / 3;
	details.political_partiesInvestment = (2 * srcCountry->getPolitical_partiesInvestment() + ideaEffectMapper.getPolitical_partiesFromIdea(details.government)) / 3;
	details.libertyInvestment = (2 * srcCountry->getLibertyInvestment() + ideaEffectMapper.getLibertyFromIdea(details.government)) / 3;
	details.equalityInvestment = (2 * srcCountry->getEqualityInvestment() + ideaEffectMapper.getEqualityFromIdea(details.government)) / 3;
	details.orderInvestment = (2 * srcCountry->getOrderInvestment() + ideaEffectMapper.getOrderFromIdea(details.government)) / 3;
	details.literacyInvestment = (2 * srcCountry->getLiteracyInvestment() + ideaEffectMapper.getLiteracyFromIdea(details.government)) / 3;
	details.reactionaryInvestment = (2 * srcCountry->getReactionaryInvestment() + ideaEffectMapper.getReactionaryFromIdea(details.government)) / 3;
	details.liberalInvestment = (2 * srcCountry->getLiberalInvestment() + ideaEffectMapper.getLiberalFromIdea(details.government)) / 3;
}

void V2::Country::resolvePolitics()
{
	details.upperHouseReactionary = static_cast<int>(5 * (1 + (details.reactionaryInvestment - 5) * 20 / 100));
	details.upperHouseLiberal = static_cast<int>(10 * (1 + (details.liberalInvestment - 5) * 20 / 100));
	details.upperHouseConservative = 100 - (details.upperHouseReactionary + details.upperHouseLiberal);

	if (srcCountry->isRevolutionary())
	{
		details.upperHouseReactionary = static_cast<int>(details.upperHouseReactionary / 3);
		details.upperHouseLiberal = static_cast<int>(details.upperHouseLiberal * 3);
		details.upperHouseConservative = 100 - (details.upperHouseReactionary + details.upperHouseLiberal);
	}

	std::string ideology;

	double liberalEffect = details.liberalInvestment - 5;
	double reactionaryEffect = details.reactionaryInvestment - 5;

	if (srcCountry->isRevolutionary())
	{
		liberalEffect += 10;
	}

	if (liberalEffect >= 2 * reactionaryEffect)
	{
		ideology = "liberal";
		details.upperHouseLiberal = static_cast<int>(details.upperHouseLiberal * 1.1);
		details.upperHouseConservative = 100 - (details.upperHouseReactionary + details.upperHouseLiberal);
	}
	else if (reactionaryEffect >= 2 * liberalEffect)
	{
		ideology = "reactionary";
		details.upperHouseReactionary = static_cast<int>(details.upperHouseReactionary * 1.1);
		details.upperHouseConservative = 100 - (details.upperHouseReactionary + details.upperHouseLiberal);
	}
	else
	{
		ideology = "conservative";
	}

	for (const auto& party : details.parties)
	{
		if (party.isActiveOn(date("1836.1.1")) && party.getIdeology() == ideology)
		{
			details.rulingParty = party.getName();
			break;
		}
	}
}

void V2::Country::generateRelations(std::shared_ptr<EU4::Country> srcCountry, const mappers::CountryMappings& countryMapper)
{
	for (auto srcRelation : srcCountry->getRelations())
	{
		const std::string& V2Tag = countryMapper.getV2Tag(srcRelation.first);
		if (!V2Tag.empty())
		{
			Relation newRelations(V2Tag, srcRelation.second);
			relations.insert(std::make_pair(V2Tag, newRelations));
		}
	}
}

void V2::Country::calculateLiteracy(std::shared_ptr<EU4::Country> srcCountry)
{
	details.literacy = 0.4;

	if (srcCountry->getReligion() == "protestant" ||
		srcCountry->getReligion() == "anglican" ||
		srcCountry->getReligion() == "confucian" ||
		srcCountry->getReligion() == "reformed")
	{
		details.literacy += 0.1;
	}

	if (srcCountry->hasModifier("the_school_establishment_act")) details.literacy += 0.05;
	if (srcCountry->hasModifier("sunday_schools")) details.literacy += 0.05;
	if (srcCountry->hasModifier("the_education_act")) details.literacy += 0.05;
	if (srcCountry->hasModifier("monastic_education_system")) details.literacy += 0.05;
	if (srcCountry->hasModifier("western_embassy_mission")) details.literacy += 0.05;

	// Universities grant at most 10% literacy, with either having 10 or when having them in 10% of provinces, whichever comes sooner.
	// Colleges do half of what universities do.

	std::vector<EU4::Province*> provinces = srcCountry->getProvinces();
	int numProvinces = provinces.size();
	int numColleges = 0;
	int numUniversities = 0;
	
	for (const auto& province : provinces)
	{
		if (province->hasBuilding("college")) numColleges++;
		if (province->hasBuilding("university")) numUniversities++;
	}

	double collegeBonus1 = 0;
	double universityBonus1 = 0;
	if (numProvinces > 0)
	{
		collegeBonus1 = static_cast<double>(numColleges) / numProvinces;
		universityBonus1 = static_cast<double>(numUniversities) * 2 / numProvinces;
	}
	double collegeBonus2 = numColleges * 0.005;
	double universityBonus2 = numUniversities * 0.01;

	double collegeBonus = std::min(std::max(collegeBonus1, collegeBonus2), 0.05);
	double universityBonus = std::min(std::max(universityBonus1, universityBonus2), 0.1);

	details.literacy += collegeBonus + universityBonus;

	if (details.literacy > theConfiguration.getMaxLiteracy())
	{
		details.literacy = theConfiguration.getMaxLiteracy();
	}

	// Finally apply collective national literacy modifier.

	details.literacy *= 1 + (details.literacyInvestment - 5) * 10 / 100;
}

void V2::Country::determineTechSchool(const mappers::TechSchoolMapper& techSchoolMapper)
{
	details.techSchool = techSchoolMapper.findBestTechSchool(
		details.armyInvestment - 5,
		details.commerceInvestment - 5,
		details.cultureInvestment - 5,
		details.industryInvestment - 5,
		details.navyInvestment - 5
	);
}

void V2::Country::buildCanals(std::shared_ptr<EU4::Country> srcCountry)
{
	for (const auto& prov : srcCountry->getProvinces())
	{
		if (prov->hasGreatProject("suez_canal")) decisions.insert("build_suez_canal");
		if (prov->hasGreatProject("kiel_canal")) decisions.insert("build_kiel_canal");
		if (prov->hasGreatProject("panama_canal")) decisions.insert("build_panama_canal");
	}
}

// used only for countries which are NOT converted (i.e. unions, dead countries, etc)
void V2::Country::initFromHistory(const mappers::Unreleasables& unreleasablesMapper)
{
	// Ping unreleasable_tags for this country's TAG
	details.isReleasableVassal = unreleasablesMapper.isTagReleasable(tag);

	// Don't fire up Details if there's no point.
	auto possibleFilename = Utils::GetFileFromTag("./blankMod/output/history/countries/", tag);
	if (!possibleFilename) possibleFilename = Utils::GetFileFromTag(theConfiguration.getVic2Path() + "/history/countries/", tag);
	if (!possibleFilename)
	{
		std::string countryName = commonCountryFile;
		const int lastSlash = countryName.find_last_of("/");
		countryName = countryName.substr(lastSlash + 1, countryName.size());
		details.filename = tag + " - " + countryName;
		return;
	}
	details = mappers::CountryDetails(*possibleFilename);
}

void V2::Country::addProvince(std::shared_ptr<Province> _province)
{
	auto itr = provinces.find(_province->getID());
	if (itr != provinces.end()) LOG(LogLevel::Error) << "Inserting province " << _province->getID() << " multiple times (addProvince())";
	provinces.insert(make_pair(_province->getID(), _province));
}

void V2::Country::addState(std::shared_ptr<State> newState, const mappers::PortProvinces& portProvincesMapper)
{
	int highestNavalLevel = 0;
	unsigned int hasHighestLevel = -1;

	states.push_back(newState);
	auto newProvinces = newState->getProvinces();

	std::vector<int> newProvinceNums;
	for (const auto& province : newProvinces)
	{
		newProvinceNums.push_back(province->getID());
	}
	auto portProvinces = Army::getPortProvinces(newProvinceNums, provinces, portProvincesMapper);

	for (unsigned int i = 0; i < newProvinces.size(); i++)
	{
		auto itr = provinces.find(newProvinces[i]->getID());
		if (itr == provinces.end())
		{
			provinces.insert(make_pair(newProvinces[i]->getID(), newProvinces[i]));
		}

		// find the province with the highest naval base level		
		const auto isPortProvince = std::find(portProvinces.begin(), portProvinces.end(), newProvinces[i]->getID()) != portProvinces.end();
		if (newProvinces[i]->getNavalBaseLevel() > highestNavalLevel&& isPortProvince)
		{
			highestNavalLevel = newProvinces[i]->getNavalBaseLevel();
			hasHighestLevel = i;
		}
		newProvinces[i]->setNavalBaseLevel(0);
	}
	if (highestNavalLevel > 0)
	{
		newProvinces[hasHighestLevel]->setNavalBaseLevel(highestNavalLevel);
	}
}

void V2::Country::convertLeaders(mappers::LeaderTraitMapper& leaderTraitMapper)
{
	if (srcCountry == nullptr) return;
	if (provinces.empty()) return;
	auto eu4Leaders = srcCountry->getMilitaryLeaders();
	if (eu4Leaders.empty()) return;

	for (const auto& eu4Leader : eu4Leaders)
	{
		Leader newLeader(eu4Leader, leaderTraitMapper);
		leaders.push_back(newLeader);
	}
}

void V2::Country::convertUncivReforms(int techGroupAlgorithm, double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper)
{
	enum civConversion { older, newer };
	switch (techGroupAlgorithm)
	{
	case(older):
		oldCivConversionMethod();
		break;
	case(newer):
		newCivConversionMethod(topTech, topInstitutions, techGroupsMapper);
		break;
	default:
		break;
	}
}

// civilisation level conversion method for games up to 1.18
void V2::Country::oldCivConversionMethod() 
{
	if (!srcCountry) return;
	double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
	double militaryDev = srcCountry->getMilTech() / totalTechs;
	double socioEconDev = srcCountry->getAdmTech() / totalTechs;

	if (srcCountry->getTechGroup() == "western" || 
		srcCountry->getTechGroup() == "high_american" || 
		srcCountry->getTechGroup() == "eastern" || 
		srcCountry->getTechGroup() == "ottoman" || 
		srcCountry->numEmbracedInstitutions() >= 7)//civilised, do nothing
	{
		details.civilized = true;
	}
	else if ((srcCountry->getTechGroup() == "north_american" || 
		srcCountry->getTechGroup() == "mesoamerican" || 
		srcCountry->getTechGroup() == "south_american" || 
		srcCountry->getTechGroup() == "new_world" || 
		srcCountry->getTechGroup() == "andean") && srcCountry->numEmbracedInstitutions() <= 3)
	{
		uncivReforms = UncivReforms(0, militaryDev, socioEconDev, this);
	}
	else if (srcCountry->getIsolationism() == 0 && srcCountry->numEmbracedInstitutions() >= 6)
	{
		uncivReforms = UncivReforms(50, militaryDev, socioEconDev, this);
	}
	else if (srcCountry->getTechGroup() == "muslim" || srcCountry->numEmbracedInstitutions() >= 6)
	{
		uncivReforms = UncivReforms(44, militaryDev, socioEconDev, this);
	}
	else if (srcCountry->getTechGroup() == "indian" || srcCountry->getIsolationism() == 0)
	{
		uncivReforms = UncivReforms(40, militaryDev, socioEconDev, this);
	}
	else if (srcCountry->getTechGroup() == "chinese" || srcCountry->numEmbracedInstitutions() == 5)
	{
		uncivReforms = UncivReforms(36, militaryDev, socioEconDev, this);
	}
	else if (srcCountry->getTechGroup() == "nomad_group")
	{
		uncivReforms = UncivReforms(30, militaryDev, socioEconDev, this);
	}
	else if (srcCountry->getTechGroup() == "sub_saharan" || 
		srcCountry->getTechGroup() == "central_african" || 
		srcCountry->getTechGroup() == "east_african" || 
		srcCountry->numEmbracedInstitutions() == 4)
	{
		uncivReforms = UncivReforms(20, militaryDev, socioEconDev, this);
	}
	else
	{
		LOG(LogLevel::Warning) << "Unhandled tech group (" << srcCountry->getTechGroup() << " with " << srcCountry->numEmbracedInstitutions() << " institutions) for " << tag << " - giving no reforms";
		uncivReforms = UncivReforms(0, militaryDev, socioEconDev, this);
	}
}

// civilisation level conversion method for games 1.19+
void V2::Country::newCivConversionMethod(double topTech, int topInsitutions, const mappers::TechGroupsMapper& techGroupsMapper) 
{
	if (!srcCountry) return;
	double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech() + srcCountry->getDipTech();

	// set civilisation cut off for 6 techs behind the the tech leader (30 years behind tech)
	// set number for civilisation level based on techs and institutions
	// at 31 techs behind completely unciv
	// each institution behind is equivalent to 2 techs behind

	double civLevel = (totalTechs + 31 - topTech) * 4;
	civLevel = civLevel + (static_cast<double>(srcCountry->numEmbracedInstitutions()) - topInsitutions) * 8;
	if (civLevel > 100) civLevel = 100;
	if (civLevel < 0) civLevel = 0;

	std::string techGroup = srcCountry->getTechGroup();

	if (theConfiguration.getEuroCentrism() == Configuration::EUROCENTRISM::EuroCentric)
	{
		details.literacy *= 1 + (static_cast<double>(techGroupsMapper.getLiteracyFromTechGroup(techGroup)) - 5.0) * 10.0 / 100.0;
		civLevel = civLevel * (static_cast<double>(techGroupsMapper.getWesternizationFromTechGroup(techGroup)) / 10.0);
	}

	details.literacy *= theConfiguration.getMaxLiteracy() * (pow(10, civLevel / 100 * 0.9 + 0.1) / 10);

	if (civLevel == 100) details.civilized = true;

	if (details.civilized == false)
	{
		totalTechs = totalTechs - srcCountry->getDipTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		uncivReforms = UncivReforms(lround(civLevel), militaryDev, socioEconDev, this);
	}
}

void V2::Country::addRailroadtoCapitalState()
{
	for (auto state: states)
	{
		if (state->provInState(details.capital))
		{
			state->addRailroads();
			break;
		}
	}
}

void V2::Country::convertLandlessReforms(std::shared_ptr<Country> capOwner) // Use current capital owner to set up.
{
	if (capOwner->isCivilized())
	{
		details.civilized = true;
	}
	else
	{
		uncivReforms = capOwner->getUncivReforms();
	}
}

void V2::Country::setArmyTech(double normalizedScore)
{
	if (!details.civilized) return;

	if (normalizedScore >= -1.0)
	{
		techs.insert("flintlock_rifles");
		inventions.insert("flintlock_rifle_armament");
	}
	if (normalizedScore >= -0.9)
	{
		techs.insert("bronze_muzzle_loaded_artillery");
	}
	if (normalizedScore >= -0.2)
	{
		techs.insert("post_napoleonic_thought");
		inventions.insert("post_napoleonic_army_doctrine");
	}
	if (normalizedScore >= 0.2)
	{
		techs.insert("army_command_principle");
	}
	if (normalizedScore >= 0.6)
	{
		techs.insert("military_staff_system");
		inventions.insert("cuirassier_activation");
		inventions.insert("dragoon_activation");
		inventions.insert("_hussar_activation");
	}
	if (normalizedScore >= 1.0)
	{
		techs.insert("army_professionalism");
		inventions.insert("army_academic_training");
		inventions.insert("field_training");
		inventions.insert("army_societal_status");
	}
}

void V2::Country::setNavyTech(double normalizedScore)
{
	if (!details.civilized) return;

	if (normalizedScore >= 0)
	{
		techs.insert("post_nelsonian_thought");
		inventions.insert("long_range_fire_tactic");
		inventions.insert("speedy_maneuvering_tactic");
	}
	if (normalizedScore >= 0.036)
	{
		techs.insert("the_command_principle");
	}
	if (normalizedScore >= 0.571)
	{
		techs.insert("clipper_design");
		techs.insert("naval_design_bureaus");
		techs.insert("alphabetic_flag_signaling");
		inventions.insert("building_station_shipyards");
	}
	if (normalizedScore >= 0.857)
	{
		techs.insert("battleship_column_doctrine");
		techs.insert("steamers");
		inventions.insert("long_range_fire_tactic");
		inventions.insert("speedy_maneuvering_tactic");
		inventions.insert("mechanized_fishing_vessels");
		inventions.insert("steamer_automatic_construction_plants");
		inventions.insert("steamer_transports");
		inventions.insert("commerce_raiders");
	}
	if (normalizedScore >= 1.0)
	{
		techs.insert("naval_professionalism");
		inventions.insert("academic_training");
		inventions.insert("combat_station_training");
		inventions.insert("societal_status");
	}
}

void V2::Country::setCommerceTech(double normalizedScore)
{
	if (!details.civilized) return;

	techs.insert("no_standard");
	if (normalizedScore >= -0.777)
	{
		techs.insert("guild_based_production");
	}
	if (normalizedScore >= -0.555)
	{
		techs.insert("private_banks");
	}
	if (normalizedScore >= -0.333)
	{
		techs.insert("early_classical_theory_and_critique");
	}
	if (normalizedScore >= -.277)
	{
		techs.insert("freedom_of_trade");
		inventions.insert("john_ramsay_mcculloch");
		inventions.insert("nassau_william_sr");
		inventions.insert("james_mill");
	}
	if (normalizedScore >= 0.333)
	{
		techs.insert("stock_exchange");
		inventions.insert("multitude_of_financial_instruments");
		inventions.insert("insurance_companies");
		inventions.insert("regulated_buying_and_selling_of_stocks");
	}
	if (normalizedScore >= 0.777)
	{
		techs.insert("ad_hoc_money_bill_printing");
		techs.insert("market_structure");
		inventions.insert("silver_standard");
		inventions.insert("decimal_monetary_system");
		inventions.insert("polypoly_structure");
		inventions.insert("oligopoly_structure");
		inventions.insert("monopoly_structure");
	}

	if (normalizedScore >= 1.0)
	{
		techs.insert("late_classical_theory");
		inventions.insert("john_elliot_cairnes");
		inventions.insert("robert_torrens");
		inventions.insert("john_stuart_mill");
	}
}


void V2::Country::setIndustryTech(double normalizedScore)
{
	if (!details.civilized) return;

	if (normalizedScore >= -1.0)
	{
		techs.insert("water_wheel_power");
		inventions.insert("tulls_seed_drill");
	}
	if (normalizedScore >= -0.714)
	{
		techs.insert("publishing_industry");
	}
	if (normalizedScore >= -0.143)
	{
		techs.insert("mechanized_mining");
		techs.insert("basic_chemistry");
		inventions.insert("ammunition_production");
		inventions.insert("small_arms_production");
		inventions.insert("explosives_production");
		inventions.insert("artillery_production");
	}
	if (normalizedScore >= 0.143)
	{
		techs.insert("practical_steam_engine");
		inventions.insert("rotherham_plough");
	}
	if (normalizedScore >= 0.428)
	{
		techs.insert("experimental_railroad");
	}
	if (normalizedScore >= 0.714)
	{
		techs.insert("mechanical_production");
		inventions.insert("sharp_n_roberts_power_loom");
		inventions.insert("jacquard_power_loom");
		inventions.insert("northrop_power_loom");
		inventions.insert("mechanical_saw");
		inventions.insert("mechanical_precision_saw");
		inventions.insert("hussey_n_mccormicks_reaping_machine");
		inventions.insert("pitts_threshing_machine");
		inventions.insert("mechanized_slaughtering_block");
		inventions.insert("precision_work");
	}
	if (normalizedScore >= 1.0)
	{
		techs.insert("clean_coal");
		inventions.insert("pit_coal");
		inventions.insert("coke");
	}
}

void V2::Country::setCultureTech(double normalizedScore)
{
	if (!details.civilized) return;

	techs.insert("classicism_n_early_romanticism");
	inventions.insert("carlism");
	techs.insert("late_enlightenment_philosophy");
	inventions.insert("declaration_of_the_rights_of_man");
	if (normalizedScore >= -0.333)
	{
		techs.insert("enlightenment_thought");
		inventions.insert("paternalism");
		inventions.insert("constitutionalism");
		inventions.insert("atheism");
		inventions.insert("egalitarianism");
		inventions.insert("rationalism");
		inventions.insert("caste_privileges");
		inventions.insert("sati_abolished");
		inventions.insert("pig_fat_cartridges");
	}
	if (normalizedScore >= 0.333)
	{
		techs.insert("malthusian_thought");
	}
	if (normalizedScore >= 0.333)
	{
		techs.insert("introspectionism");
	}
	if (normalizedScore >= 0.666)
	{
		techs.insert("romanticism");
		inventions.insert("romanticist_literature");
		inventions.insert("romanticist_art");
		inventions.insert("romanticist_music");
	}
}

bool V2::Country::addFactory(std::shared_ptr<Factory> factory)
{
	// check factory techs
	std::string requiredTech = factory->getRequiredTech();
	if (!requiredTech.empty())
	{
		if (!techs.count(requiredTech)) return false;
	}

	// check factory inventions
	std::string requiredInvention = factory->getRequiredInvention();
	if (!requiredInvention.empty())
	{
		if (!inventions.count(requiredInvention)) return false;
	}

	// find a state to add the factory to, which meets the factory's requirements
	std::vector<std::pair<double, std::shared_ptr<State>>> candidates;
	for (const auto& candidate: states)
	{
		if (candidate->isColonial()) continue;
		if (candidate->getFactoryCount() >= 8) continue;
		if (factory->requiresCoastal()) if (!candidate->isCoastal()) continue;
		if (!candidate->hasLandConnection()) continue;

		double candidateScore = candidate->getSuppliedInputs(factory) * 100;
		candidateScore -= static_cast<double>(candidate->getFactoryCount()) * 10;
		candidateScore += candidate->getMfgRatio();
		candidates.emplace_back(std::pair<double, std::shared_ptr<State>>(candidateScore, candidate));
	}

	sort(candidates.begin(), candidates.end(), factoryCandidateSortPredicate);

	if (candidates.empty()) return false;

	candidates[0].second->addFactory(factory);
	details.numFactories++;
	return true;
}

bool V2::Country::factoryCandidateSortPredicate(const std::pair<double, std::shared_ptr<State>>& lhs, const std::pair<double, std::shared_ptr<State>>& rhs)
{
	if (lhs.first != rhs.first) return lhs.first > rhs.first;
	return lhs.second->getID() < rhs.second->getID();
}

void V2::Country::setupPops(
	double popWeightRatio,
	int popConversionAlgorithm,
	const mappers::ProvinceMapper& provinceMapper
) {
	// skip entirely for empty nations
	if (states.empty()) return;

	// create the pops
	for (const auto& province: provinces) province.second->doCreatePops(popWeightRatio, this, popConversionAlgorithm, provinceMapper);
}

void V2::Country::convertArmies(
	const mappers::RegimentCostsMapper& regimentCostsMapper,
	const std::map<int, std::shared_ptr<V2::Province>>& allProvinces,
	const mappers::PortProvinces& portProvincesMapper,
	const mappers::ProvinceMapper& provinceMapper
) {
	if (srcCountry == nullptr) return;
	if (provinces.empty()) return;

	// set up armies with whatever regiments they deserve, rounded down
	// and keep track of the remainders for later
	for (auto& eu4army : srcCountry->getArmies())
	{
		Army army(eu4army, tag, details.civilized, regimentCostsMapper, allProvinces, provinceMapper, portProvincesMapper, unitNameCount, localisation.getLocalAdjective());
		if (army.success()) armies.push_back(army); // That went well.
		// copy over remainders, if any.
		auto armyRemainders = army.getArmyRemainders();
		for (const auto& remainder : armyRemainders) countryRemainders[remainder.first] += remainder.second;
	}

	// allocate the remainders from the whole country to the armies according to their need, rounding up
	for (auto& remainder : countryRemainders)
	{
		while (remainder.second > 0.0)
		{
			Army* army = getArmyForRemainder(remainder.first);
			if (army == nullptr) break;

			switch (army->addRegimentToArmy(remainder.first, allProvinces, provinceMapper, portProvincesMapper, unitNameCount, localisation.getLocalAdjective()))
			{
			case addRegimentToArmyResult::success:
				remainder.second -= 1.0;
				army->addRegimentRemainder(remainder.first, -1.0);
				break;
			case addRegimentToArmyResult::retry:
				break;
			case addRegimentToArmyResult::fail:
				army->addRegimentRemainder(remainder.first, -2000.0);
				break;
			}
		}
	}
}

// find the army most in need of a regiment of this category
V2::Army* V2::Country::getArmyForRemainder(REGIMENTTYPE chosenType)
{
	Army* retval = nullptr;
	double retvalRemainder = -1000.0;
	for (auto& army : armies)
	{
		// only add units to armies that originally had units of the same category
		if (army.hasRegimentsOfType(chosenType))
		{
			if (army.getRegimentRemainder(chosenType) > retvalRemainder)
			{
				retvalRemainder = army.getRegimentRemainder(chosenType);
				retval = &army;
			}
		}
	}

	return retval;
}

std::string	V2::Country::getColonialRegion()
{
	if (!srcCountry) return "";
	return srcCountry->getColonialRegion();
}
