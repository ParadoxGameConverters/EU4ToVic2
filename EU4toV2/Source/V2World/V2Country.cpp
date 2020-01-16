#include "V2Country.h"
#include "Log.h"
#include "../Configuration.h"
#include "OSCompatibilityLayer.h"
#include "../EU4World/Country/EU4Country.h"
#include "../EU4World/Leader/EU4Leader.h"
#include "../EU4World/Provinces/EU4Province.h"
#include "../Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "../Mappers/TechGroups/TechGroupsMapper.h"
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "V2World.h"
#include "V2State.h"
#include "V2Province.h"
#include "V2Creditor.h"
#include <algorithm>
#include <fstream>
#include <queue>
#include <cmath>
#include "ParserHelpers.h"
#include "Reforms/Reforms.h"
#include "Reforms/UncivReforms.h"

const int MONEYFACTOR = 30;	// ducat to pound conversion rate


V2Country::V2Country(const string& countriesFileLine, const V2World* _theWorld, bool _dynamicCountry)
{
	registerKeyword(std::regex("party"), [this](const std::string& unused, std::istream& theStream)
		{
			mappers::PartyType newPartyType(theStream);
			V2::Party newParty(newPartyType);
			parties.push_back(newParty);
		});
	registerKeyword(std::regex("primary_culture"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString primCulStr(theStream);
			primaryCulture = primCulStr.getString();
		});
	registerKeyword(std::regex("religion"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString religionStr(theStream);
			religion = religionStr.getString();
		});
	registerKeyword(std::regex("government"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString governmentStr(theStream);
			government = governmentStr.getString();
		});
	registerKeyword(std::regex("civilized"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ignored(theStream);
			civilized = true;
		});
	registerKeyword(std::regex("is_releasable_vassal"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString releasableStr(theStream);
			isReleasableVassal = (releasableStr.getString() == "yes");
		});
	registerKeyword(std::regex("nationalvalue"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nationalvalueStr(theStream);
			nationalValue = nationalvalueStr.getString();
		});
	registerKeyword(std::regex("capital"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt capitalInt(theStream);
			capital = capitalInt.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	string filename;
	int start = countriesFileLine.find_first_of('/');
	start++;
	int size = countriesFileLine.find_last_of('\"') - start;
	filename = countriesFileLine.substr(start, size);

	if (Utils::DoesFileExist("./blankMod/output/common/countries/" + filename))
	{
		parseFile("./blankMod/output/common/countries/" + filename);
	}
	else if (Utils::DoesFileExist(theConfiguration.getVic2Path() + "/common/countries/" + filename))
	{
		parseFile(theConfiguration.getVic2Path() + "/common/countries/" + filename);
	}
	else
	{
		LOG(LogLevel::Debug) << "Could not find file common/countries/" << filename << " - skipping";
	}

	theWorld			= _theWorld;
	newCountry		= false;
	dynamicCountry	= _dynamicCountry;

	tag = countriesFileLine.substr(0, 3);
	commonCountryFile	= localisation.convert(filename);
	rulingParty			= "";

	states.clear();
	provinces.clear();

	leadership				= 0.0;
	plurality				= 0.0;
	capital					= 0;
	diploPoints				= 0.0;
	badboy					= 0.0;
	money						= 0.0;
	techSchool				= "traditional_academic";
	researchPoints			= 0.0;
	civilized				= false;
	isReleasableVassal	= true;
	inHRE						= false;
	holyRomanEmperor		= false;
	celestialEmperor		= false;
	primaryCulture			= "dummy";
	religion					= "shamanist";
	government				= "absolute_monarchy";
	nationalValue			= "nv_order";
	lastBankrupt			= date();
	bankReserves			= 0.0;
	literacy					= 0.0;

	acceptedCultures.clear();
	techs.clear();
	reactionaryIssues.clear();
	conservativeIssues.clear();
	liberalIssues.clear();
	creditors.clear();

	reforms		= nullptr;
	srcCountry	= nullptr;

	upperHouseReactionary	= 10;
	upperHouseConservative	= 65;
	upperHouseLiberal			= 25;

	uncivReforms = nullptr;

	if (parties.empty())
	{	// No parties are specified. Grab some.
		loadPartiesFromBlob(theWorld->getPartyNameMapper(), theWorld->getPartyTypeMapper());
	}

	// set a default ruling party
	for (const auto& party : parties)
	{
		if (party.isActiveOn(date("1836.1.1")))
		{
			rulingParty = party.getName();
			break;
		}
	}

	colonyOverlord = nullptr;

	numFactories	= 0;

}

void V2Country::loadPartiesFromBlob(const mappers::PartyNameMapper& partyNameMapper, const mappers::PartyTypeMapper& partyTypeMapper)
{

	auto partyMap = partyNameMapper.getMap();

	map<string, mappers::PartyName>::iterator partyItr;

	size_t i = 0;
	for (partyItr = partyMap.begin(); partyItr != partyMap.end(); ++partyItr)
	{
		map<string, string>::iterator languageItr;
		auto languageMap = partyItr->second.getMap();

		std::string partyKey = tag + '_' + partyItr->first;

		auto partyType = partyTypeMapper.getPartyTypeByIdeology(partyItr->first);
		if (!partyType)
		{
			Log(LogLevel::Warning) << "Party type " << partyItr->first << " has no entry in party_blobs.txt!";
			continue;
		}
		partyType->setName(partyKey);
		V2::Party newParty(*partyType);
		parties.push_back(newParty);
		localisation.setPartyKey(i, partyKey);
		
		for (languageItr = languageMap.begin(); languageItr != languageMap.end(); ++languageItr)
		{
			localisation.setPartyName(i, languageItr->first, languageItr->second);
		}
		++i;
	}

}


V2Country::V2Country(const string& _tag, const string& _commonCountryFile, const V2World* _theWorld)
{
	theWorld = _theWorld;
	newCountry = true;
	dynamicCountry = false;

	tag					= _tag;
	commonCountryFile	= localisation.convert(_commonCountryFile);
	std::replace(commonCountryFile.begin(), commonCountryFile.end(), ':', ';');
	std::replace(commonCountryFile.begin(), commonCountryFile.end(), '/', ' ');
	std::replace(commonCountryFile.begin(), commonCountryFile.end(), '\\', ' ');
	commonCountryFile = commonCountryFile;
	parties.clear();
	rulingParty			= "";

	states.clear();
	provinces.clear();
	inventions.clear();

	leadership				= 0.0;
	plurality				= 0.0;
	capital					= 0;
	diploPoints				= 0.0;
	badboy					= 0.0;
	money						= 0.0;
	techSchool				= "traditional_academic";
	researchPoints			= 0.0;
	civilized				= false;
	isReleasableVassal	= true;
	inHRE						= false;
	holyRomanEmperor		= false;
	celestialEmperor		= false;
	primaryCulture			= "dummy";
	religion					= "shamanist";
	government				= "absolute_monarchy";
	nationalValue			= "nv_order";
	lastBankrupt			= date();
	bankReserves			= 0.0;
	literacy					= 0.0;

	acceptedCultures.clear();
	techs.clear();
	reactionaryIssues.clear();
	conservativeIssues.clear();
	liberalIssues.clear();
	creditors.clear();

	reforms		= nullptr;
	srcCountry	= nullptr;

	upperHouseReactionary	= 10;
	upperHouseConservative	= 65;
	upperHouseLiberal			= 25;

	uncivReforms = nullptr;

	if (parties.empty())
	{	// No parties are specified. Get some.
		loadPartiesFromBlob(theWorld->getPartyNameMapper(), theWorld->getPartyTypeMapper());
	}

	// set a default ruling party
	for (const auto& party : parties)
	{
		if (party.isActiveOn(date("1836.1.1")))
		{
			rulingParty = party.getName();
			break;
		}
	}

	colonyOverlord = nullptr;

	numFactories	= 0;
}


void V2Country::output() const
{
	if(!dynamicCountry)
	{
		ofstream output;		
		output.open("output/" + theConfiguration.getOutputName() + "/history/countries/" + filename);
		if (!output.is_open())
		{
			throw std::runtime_error("Could not create country history file " + filename);
		}

		if (capital > 0)
		{
			output << "capital=" << capital << "\n";
		}
		output << "primary_culture = " << primaryCulture << "\n";
		for (set<string>::iterator i = acceptedCultures.begin(); i != acceptedCultures.end(); ++i)
		{
			output << "culture = " << *i << "\n";
		}
		output << "religion = " << religion << "\n";
		output << "government = " << government << "\n";
		output << "plurality= " << plurality << "\n";
		output << "nationalvalue= " << nationalValue << "\n";
		output << "literacy= " << literacy << "\n";
		if (civilized)
		{
			output << "civilized=yes\n";
		}
		if (!isReleasableVassal)
		{
			output << "is_releasable_vassal=no\n";
		}
		output << "\n";
		output << "# Social Reforms\n";
		output << "wage_reform = no_minimum_wage\n";
		output << "work_hours = no_work_hour_limit\n";
		output << "safety_regulations = no_safety\n";
		output << "health_care = no_health_care\n";
		output << "unemployment_subsidies = no_subsidies\n";
		output << "pensions = no_pensions\n";
		output << "school_reforms = no_schools\n";

		if (reforms != nullptr)
		{
			output << *reforms;
		}
		else
		{
			output << "# Political Reforms\n";
			output << "slavery=yes_slavery\n";
			output << "vote_franschise=none_voting\n";
			output << "upper_house_composition=appointed\n";
			output << "voting_system=jefferson_method\n";
			output << "public_meetings=yes_meeting\n";
			output << "press_rights=censored_press\n";
			output << "trade_unions=no_trade_unions\n";
			output << "political_parties=underground_parties\n";
		}
		output << "\n";
		output << "ruling_party= " << rulingParty << "\n";
		output << "upper_house=\n";
		output << "{\n";
		output << "	fascist = 0\n";
		output << "	liberal = " << upperHouseLiberal << "\n";
		output << "	conservative = " << upperHouseConservative << "\n";
		output << "	reactionary = " << upperHouseReactionary << "\n";
		output << "	anarcho_liberal = 0\n";
		output << "	socialist = 0\n";
		output << "	communist = 0\n";
		output << "}\n";
		output << "\n";
		output << "# Starting Consciousness\n";
		output << "consciousness = 0\n";
		output << "nonstate_consciousness = 0\n";
		output << "\n";
		outputTech(output);
		if (!civilized)
		{
			if (uncivReforms != nullptr)
			{
				output << *uncivReforms;
			}
		}
		output << "prestige=" << prestige << "\n";

		if (!decisions.empty())
		{
			output << "\n";
			output << "# Decisions\n";
			output << "1.1.1 = {\n";
			for (const auto& decision : decisions)
			{
				output << "\tdecision = " << decision << "\n";
			}
			output << "}\n";
		}

		//output << "	schools=\"%s\"\n", techSchool.c_str());

		output << "oob = \"" << (tag + "_OOB.txt") << "\"\n";

		if (holyRomanEmperor)
		{
			output << "set_country_flag = emperor_hre\n";
		}
		else if (inHRE)
		{
			output << "set_country_flag = member_hre\n";
		}

		if (celestialEmperor)
		{
			output << "set_country_flag = celestial_emperor\n";
		}

		output.close();

		outputOOB();
	}

	if (newCountry)
	{
		// Output common country file.
		std::ofstream commonCountryOutput("output/" + theConfiguration.getOutputName() + "/common/countries/" + commonCountryFile);
		if (!commonCountryOutput.is_open())
		{
			LOG(LogLevel::Error) << "Could not open output/" + theConfiguration.getOutputName() + "/common/countries/" + commonCountryFile;
			exit(-1);
		}
		commonCountryOutput << "graphical_culture = UsGC\n";	// default to US graphics
		commonCountryOutput << "color = { " << nationalColors.getMapColor() << " }\n";
		for (const auto& party : parties) commonCountryOutput << party;
	}
}


void V2Country::outputToCommonCountriesFile(FILE* output) const
{
	fprintf(output, "%s = \"countries/%s\"\n", tag.c_str(), commonCountryFile.c_str());
}


void V2Country::outputLocalisation(std::ostream& output) const
{
	 output << localisation;
}


void V2Country::outputTech(std::ofstream& output) const
{
	output << "\n";
	output << "# Technologies\n";
	for (vector<string>::const_iterator itr = techs.begin(); itr != techs.end(); ++itr)
	{
		output << *itr << "= 1\n";
	}
}


void V2Country::outputElection(FILE* output) const
{
	date electionDate = date("1832.1.1");
	fprintf(output, "	last_election=%s\n", electionDate.toString().c_str());
}


void V2Country::outputOOB() const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/units/" + tag + "_OOB.txt");
	if (!output.is_open())
	{
		throw std::runtime_error("Could not create OOB file " + tag + "_OOB.txt");
	}

	output << "#Sphere of Influence\n";
	output << "\n";
	for (auto relation: relations)
	{
		output << relation.second;
	}

	output << "\n";
	output << "#Leaders\n";
	for (auto leader: leaders)
	{
		output << leader;
	}

	output << "\n";
	output << "#Armies\n";
	for (auto army: armies)
	{
		output << army;
	}

	output.close();
}


void V2Country::initFromEU4Country(
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

	if (false == srcCountry->getRandomName().empty())
	{
		newCountry = true;
	}

	auto possibleFilename = Utils::GetFileFromTag("./blankMod/output/history/countries/", tag);
	if (!possibleFilename)
	{
		possibleFilename = Utils::GetFileFromTag(theConfiguration.getVic2Path() + "/history/countries/", tag);
	}

	if (!possibleFilename)
	{
		string countryName	= commonCountryFile;
		int lastSlash			= countryName.find_last_of("/");
		countryName				= countryName.substr(lastSlash + 1, countryName.size());
		filename					= tag + " - " + countryName;
	}
	else
	{
		filename = *possibleFilename;
	}

	// Color
	nationalColors = srcCountry->getNationalColors();

	// Localisation
	localisation.setTag(tag);
	localisation.readFromCountry(*srcCountry);

	// Capital
	int oldCapital = srcCountry->getCapital();
	auto potentialCapitals = provinceMapper.getVic2ProvinceNumbers(oldCapital);
	if (potentialCapitals.size() > 0)
	{
		capital = *potentialCapitals.begin();
	}

	// in HRE
	inHRE					= srcCountry->getInHRE();
	holyRomanEmperor	= srcCountry->getHolyRomanEmperor();

	// celestial emperor
	celestialEmperor = srcCountry->getCelestialEmperor();

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
	reforms = new V2::Reforms(this, srcCountry);

	// Relations
	generateRelations(_srcCountry, countryMapper);

	// Literacy and Tech school
	calculateLiteracy(_srcCountry);
	determineTechSchool(techSchoolMapper);

	// Misc
	buildCanals(_srcCountry);
}

void V2Country::setReligion(std::shared_ptr<EU4::Country> srcCountry, const mappers::ReligionMapper& religionMapper)
{
	string srcReligion = srcCountry->getReligion();
	if (srcReligion.size() > 0)
	{
		std::optional<std::string> match = religionMapper.getVic2Religion(srcReligion);
		if (!match)
		{
			LOG(LogLevel::Warning) << "No religion mapping defined for " << srcReligion
				<< " (" << srcCountry->getTag() << " -> " << tag << ')';
		}
		else
		{
			religion = *match;
		}
	}
}

void V2Country::setPrimaryAndAcceptedCultures(std::shared_ptr<EU4::Country> srcCountry, const mappers::CultureMapper& cultureMapper, const EU4::Regions& eu4Regions)
{
	int oldCapital = srcCountry->getCapital();

	// primary culture
	string srcCulture = srcCountry->getPrimaryCulture();

	if (srcCulture.size() > 0)
	{
		std::optional<std::string> matched = cultureMapper.cultureMatch(
			eu4Regions,
			srcCulture,
			religion,
			oldCapital,
			srcCountry->getTag()
		);
		if (!matched)
		{
			LOG(LogLevel::Warning) << "No culture mapping defined for " << srcCulture
				<< " (" << srcCountry->getTag() << " -> " << tag << ')';
		}
		else
		{
			primaryCulture = *matched;
		}
	}

	//accepted cultures
	vector<string> srcAceptedCultures = srcCountry->getAcceptedCultures();
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
			religion,
			oldCapital,
			srcCountry->getTag()
		);
		if (dstCulture)
		{
			if (primaryCulture != *dstCulture)
			{
				acceptedCultures.insert(*dstCulture);
			}
		}
		else
		{
			LOG(LogLevel::Warning) << "No culture mapping defined for " << srcCulture
				<< " (" << srcCountry->getTag() << " -> " << tag << ')';
		}
	}

}

void V2Country::determineGovernmentType(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::GovernmentMapper& governmentMapper)
{
	government = governmentMapper.matchGovernment(srcCountry->getGovernment());

	for (auto reformStr : srcCountry->getReforms())
	{
		std::string enforce = ideaEffectMapper.getEnforceFromIdea(reformStr);
		if (!enforce.empty())
		{
			government = enforce;
		}
	}
	
	// almost but not quite

	if (srcCountry->isRevolutionary())
	{
		government = "bourgeois_dictatorship";
	}

}

void V2Country::finalizeInvestments(std::shared_ptr<EU4::Country> srcCountry, const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	// Collect and finalize all idea/reform/government effects. We have combined reforms + ideas incoming, but lack government component (the last 33%)
	// Resulting scores for all of these will be between 0 and 10, with 5 being average and supposed to be ignored.
	// Each point above or below 5 should alter absolute values by 10%.

	armyInvestment = (2 * srcCountry->getArmyInvestment() + ideaEffectMapper.getArmyFromIdea(government)) / 3;
	navyInvestment = (2 * srcCountry->getNavyInvestment() + ideaEffectMapper.getNavyFromIdea(government)) / 3;
	commerceInvestment = (2 * srcCountry->getCommerceInvestment() + ideaEffectMapper.getCommerceFromIdea(government)) / 3;
	industryInvestment = (2 * srcCountry->getIndustryInvestment() + ideaEffectMapper.getIndustryFromIdea(government)) / 3;
	cultureInvestment = (2 * srcCountry->getCultureInvestment() + ideaEffectMapper.getCultureFromIdea(government)) / 3;
	slaveryInvestment = (2 * srcCountry->getSlaveryInvestment() + ideaEffectMapper.getSlaveryFromIdea(government)) / 3;
	upper_house_compositionInvestment = (2 * srcCountry->getUpper_house_compositionInvestment() + ideaEffectMapper.getUpper_house_compositionFromIdea(government)) / 3;
	vote_franchiseInvestment = (2 * srcCountry->getVote_franchiseInvestment() + ideaEffectMapper.getVote_franchiseFromIdea(government)) / 3;
	voting_systemInvestment = (2 * srcCountry->getVoting_systemInvestment() + ideaEffectMapper.getVoting_systemFromIdea(government)) / 3;
	public_meetingsInvestment = (2 * srcCountry->getPublic_meetingsInvestment() + ideaEffectMapper.getPublic_meetingsFromIdea(government)) / 3;
	press_rightsInvestment = (2 * srcCountry->getPress_rightsInvestment() + ideaEffectMapper.getPress_rightsFromIdea(government)) / 3;
	trade_unionsInvestment = (2 * srcCountry->getTrade_unionsInvestment() + ideaEffectMapper.getTrade_unionsFromIdea(government)) / 3;
	political_partiesInvestment = (2 * srcCountry->getPolitical_partiesInvestment() + ideaEffectMapper.getPolitical_partiesFromIdea(government)) / 3;
	libertyInvestment = (2 * srcCountry->getLibertyInvestment() + ideaEffectMapper.getLibertyFromIdea(government)) / 3;
	equalityInvestment = (2 * srcCountry->getEqualityInvestment() + ideaEffectMapper.getEqualityFromIdea(government)) / 3;
	orderInvestment = (2 * srcCountry->getOrderInvestment() + ideaEffectMapper.getOrderFromIdea(government)) / 3;
	literacyInvestment = (2 * srcCountry->getLiteracyInvestment() + ideaEffectMapper.getLiteracyFromIdea(government)) / 3;
	reactionaryInvestment = (2 * srcCountry->getReactionaryInvestment() + ideaEffectMapper.getReactionaryFromIdea(government)) / 3;
	liberalInvestment = (2 * srcCountry->getLiberalInvestment() + ideaEffectMapper.getLiberalFromIdea(government)) / 3;

}

void V2Country::resolvePolitics()
{
	upperHouseReactionary = static_cast<int>(5 * (1 + (reactionaryInvestment - 5) * 20 / 100));
	upperHouseLiberal = static_cast<int>(10 * (1 + (liberalInvestment - 5) * 20 / 100));
	upperHouseConservative = 100 - (upperHouseReactionary + upperHouseLiberal);

	if (srcCountry->isRevolutionary())
	{
		upperHouseReactionary = static_cast<int>(upperHouseReactionary / 3);
		upperHouseLiberal = static_cast<int>(upperHouseLiberal * 3);
		upperHouseConservative = 100 - (upperHouseReactionary + upperHouseLiberal);
	}

	string idealogy;

	double liberalEffect = liberalInvestment - 5;
	double reactionaryEffect = reactionaryInvestment - 5;

	if (srcCountry->isRevolutionary())
	{
		liberalEffect += 10;
	}

	if (liberalEffect >= 2 * reactionaryEffect)
	{
		idealogy = "liberal";
		upperHouseLiberal = static_cast<int>(upperHouseLiberal * 1.1);
		upperHouseConservative = 100 - (upperHouseReactionary + upperHouseLiberal);
	}
	else if (reactionaryEffect >= 2 * liberalEffect)
	{
		idealogy = "reactionary";
		upperHouseReactionary = static_cast<int>(upperHouseReactionary * 1.1);
		upperHouseConservative = 100 - (upperHouseReactionary + upperHouseLiberal);
	}
	else
	{
		idealogy = "conservative";
	}

	for (const auto& party : parties)
	{
		if (party.isActiveOn(date("1836.1.1")) && (party.getIdeology() == idealogy))
		{
			rulingParty = party.getName();
			break;
		}
	}
}

void V2Country::generateRelations(std::shared_ptr<EU4::Country> srcCountry, const mappers::CountryMappings& countryMapper)
{
	auto srcRelations = srcCountry->getRelations();
	for (auto srcRelation : srcRelations)
	{
		const std::string& V2Tag = countryMapper.getV2Tag(srcRelation.first);
		if (!V2Tag.empty())
		{
			V2::Relation newRelations(V2Tag, srcRelation.second);
			relations.insert(std::make_pair(V2Tag, newRelations));
		}
	}
}

void V2Country::calculateLiteracy(std::shared_ptr<EU4::Country> srcCountry)
{
	literacy = 0.4;

	if (
		((srcCountry->getReligion().compare("protestant") == 0) ||
		(srcCountry->getReligion().compare("anglican") == 0) ||
			(srcCountry->getReligion().compare("confucian") == 0) ||
			(srcCountry->getReligion().compare("reformed") == 0))
		)
	{
		literacy += 0.1;
	}

	if (srcCountry->hasModifier("the_school_establishment_act"))
	{
		literacy += 0.05;
	}
	if (srcCountry->hasModifier("sunday_schools"))
	{
		literacy += 0.05;
	}
	if (srcCountry->hasModifier("the_education_act"))
	{
		literacy += 0.05;
	}
	if (srcCountry->hasModifier("monastic_education_system"))
	{
		literacy += 0.05;
	}
	if (srcCountry->hasModifier("western_embassy_mission"))
	{
		literacy += 0.05;
	}

	// Universities grant at most 10% literacy, with either having 10 or when having them in 10% of provinces, whichever comes sooner.
	// Colleges do half of what universities do.

	int numProvinces = 0;
	int numColleges = 0;
	int numUniversities = 0;
	vector<EU4::Province*> provinces = srcCountry->getProvinces();
	numProvinces = provinces.size();
	for (vector<EU4::Province*>::iterator i = provinces.begin(); i != provinces.end(); ++i)
	{
		if ((*i)->hasBuilding("college"))
		{
			numColleges++;
		}
		if ((*i)->hasBuilding("university"))
		{
			numUniversities++;
		}
	}

	double collegeBonus1 = 0;
	double universityBonus1 = 0;
	if (numProvinces > 0)
	{
		collegeBonus1 = numColleges / numProvinces;
		universityBonus1 = numUniversities * 2 / numProvinces;
	}
	double collegeBonus2 = numColleges * 0.005;
	double universityBonus2 = numUniversities * 0.01;

	double collegeBonus = min(max(collegeBonus1, collegeBonus2), 0.05);
	double universityBonus = min(max(universityBonus1, universityBonus2), 0.1);

	literacy += collegeBonus + universityBonus;

	if (literacy > theConfiguration.getMaxLiteracy())
	{
		literacy = theConfiguration.getMaxLiteracy();
	}

	// Finally apply collective national literacy modifier.

	literacy *= (1 + (literacyInvestment - 5) * 10 / 100);

}

void V2Country::determineTechSchool(const mappers::TechSchoolMapper& techSchoolMapper)
{
	techSchool = techSchoolMapper.findBestTechSchool(
		armyInvestment - 5,
		commerceInvestment - 5,
		cultureInvestment - 5,
		industryInvestment - 5,
		navyInvestment - 5
	);
}

void V2Country::buildCanals(std::shared_ptr<EU4::Country> srcCountry)
{
	for (const auto& prov : srcCountry->getProvinces())
	{
		if (prov->hasGreatProject("suez_canal"))
		{
			decisions.push_back("build_suez_canal");
		}
		if (prov->hasGreatProject("kiel_canal"))
		{
			decisions.push_back("build_kiel_canal");
		}
		if (prov->hasGreatProject("panama_canal"))
		{
			decisions.push_back("build_panama_canal");
		}
	}

}


// used only for countries which are NOT converted (i.e. unions, dead countries, etc)
void V2Country::initFromHistory(const mappers::Unreleasables& unreleasablesMapper)
{
	// Ping unreleasable_tags for this country's TAG
	isReleasableVassal = unreleasablesMapper.isTagReleasable(tag);

	string fullFilename;

	auto possibleFilename = Utils::GetFileFromTag("./blankMod/output/history/countries/", tag);
	if (possibleFilename)
	{
		filename = *possibleFilename;
		fullFilename = "./blankMod/output/history/countries/" + filename;
	}
	else
	{
		possibleFilename = Utils::GetFileFromTag(theConfiguration.getVic2Path() + "/history/countries/", tag);
		if (possibleFilename)
		{
			filename = *possibleFilename;
			fullFilename = theConfiguration.getVic2Path() + "/history/countries/" + filename;
		}
	}
	if (!possibleFilename)
	{
		string countryName	= commonCountryFile;
		int lastSlash			= countryName.find_last_of("/");
		countryName				= countryName.substr(lastSlash + 1, countryName.size());
		filename					= tag + " - " + countryName;
		return;
	}
	parseFile(fullFilename);
}


void V2Country::addProvince(V2Province* _province)
{
	auto itr = provinces.find(_province->getNum());
	if (itr != provinces.end())
	{
		LOG(LogLevel::Error) << "Inserting province " << _province->getNum() << " multiple times (addProvince())";
	}
	provinces.insert(make_pair(_province->getNum(), _province));
}



void V2Country::addState(V2State* newState, const mappers::PortProvinces& portProvincesMapper)
{
	int				highestNavalLevel	= 0;
	unsigned int	hasHighestLevel	= -1;
	bool				hasNavalBase		= false;

	states.push_back(newState);
	vector<V2Province*> newProvinces = newState->getProvinces();

	std::vector<int> newProvinceNums;
	for (const auto& province: newProvinces)
	{
		newProvinceNums.push_back(province->getNum());
	}
	auto portProvinces = V2::Army::getPortProvinces(newProvinceNums, provinces, portProvincesMapper);

	for (unsigned int i = 0; i < newProvinces.size(); i++)
	{
		auto itr = provinces.find(newProvinces[i]->getNum());
		if (itr == provinces.end())
		{
			provinces.insert(make_pair(newProvinces[i]->getNum(), newProvinces[i]));
		}

		// find the province with the highest naval base level
		int navalLevel = 0;
		const EU4::Province* srcProvince = newProvinces[i]->getSrcProvince();
		if (srcProvince != nullptr)
		{
			if (srcProvince->hasBuilding("shipyard"))
			{
				navalLevel += 1;
			}
			if (srcProvince->hasBuilding("grand_shipyard"))
			{
				navalLevel += 1;
			}
			if (srcProvince->hasBuilding("naval_arsenal"))
			{
				navalLevel += 1;
			}
			if (srcProvince->hasBuilding("naval_base"))
			{
				navalLevel += 1;
			}
		}
		bool isPortProvince = std::find(portProvinces.begin(), portProvinces.end(), newProvinces[i]->getNum()) != portProvinces.end();
		if (navalLevel > highestNavalLevel && isPortProvince)
		{
			highestNavalLevel	= navalLevel;
			hasHighestLevel	= i;
		}
		newProvinces[i]->setNavalBaseLevel(0);
	}
	if (highestNavalLevel > 0)
	{
		newProvinces[hasHighestLevel]->setNavalBaseLevel(1);
	}
}

void V2Country::convertLeaders(mappers::LeaderTraitMapper& leaderTraitMapper)
{
	if (srcCountry == nullptr) return;
	if (provinces.size() == 0) return;
	auto eu4Leaders = srcCountry->getMilitaryLeaders();
	if (eu4Leaders.empty()) return;

	std::vector<EU4::Leader>::iterator eu4LeaderItr;
	for (eu4LeaderItr = eu4Leaders.begin(); eu4LeaderItr != eu4Leaders.end(); ++eu4LeaderItr)
	{
		V2::Leader newLeader(*eu4LeaderItr, leaderTraitMapper);
		leaders.push_back(newLeader);
	}
}


//#define TEST_V2_PROVINCES
void V2Country::convertArmies(
	const mappers::RegimentCostsMapper& regimentCostsMapper,
	const std::map<int, V2Province*>& allProvinces,
	const mappers::PortProvinces& portProvincesMapper,
	const mappers::ProvinceMapper& provinceMapper,
	const mappers::AdjacencyMapper& adjacencyMapper
) {
#ifndef TEST_V2_PROVINCES
	if (srcCountry == nullptr) return;
	if (provinces.empty()) return;

	// set up armies with whatever regiments they deserve, rounded down
	// and keep track of the remainders for later
	for (auto& eu4army: srcCountry->getArmies())
	{
		V2::Army army(eu4army, tag, civilized, regimentCostsMapper, allProvinces, provinceMapper, portProvincesMapper, unitNameCount, localisation.getLocalAdjective());
		if (army.success()) armies.push_back(army); // That went well.
		// copy over remainders, if any.
		auto armyRemainders = army.getArmyRemainders();
		for (const auto& remainder : armyRemainders) countryRemainders[remainder.first] += remainder.second;
	}

	// allocate the remainders from the whole country to the armies according to their need, rounding up
	for (auto& remainder: countryRemainders)
	{
		while (remainder.second > 0.0)
		{
			V2::Army* army = getArmyForRemainder(remainder.first);
			if (army == nullptr) break;

			switch (army->addRegimentToArmy(remainder.first, allProvinces, provinceMapper, portProvincesMapper, unitNameCount, localisation.getLocalAdjective()))		
			{
			case V2::addRegimentToArmyResult::success:
				remainder.second -= 1.0;
				army->addRegimentRemainder(remainder.first, -1.0);
				break;
			case V2::addRegimentToArmyResult::retry:
				break;
			case V2::addRegimentToArmyResult::fail:
				army->addRegimentRemainder(remainder.first, -2000.0);
				break;
			}
		}
	}

#else // ifdef TEST_V2_PROVINCES
	// output one big ship to each V2 province that's neither whitelisted nor blacklisted, but only 10 at a time per nation
	// output from this mode is used to build whitelist and blacklist files
	set<int> port_blacklist = getPortBlacklist();
	int n_tests = 0;
	for (auto itr = provinces.begin(); (itr != provinces.end()) && (n_tests < 50); ++itr)
	{
		V2Province* pitr = itr->second;
		if (pitr->isCoastal())
		{
			set<int>::iterator black = std::find(port_blacklist.begin(), port_blacklist.end(), pitr->getNum());
			if (black != port_blacklist.end())
				continue;

			V2Army army = V2Army::makeTestNavy(itr->first);
			armies.push_back(army);

			vector<int>::iterator white = std::find(port_whitelist.begin(), port_whitelist.end(), pitr->getNum());
			if (white == port_whitelist.end())
			{
				++n_tests;
				ofstream s("port_greylist.txt", ios_base::app);
				s << pitr->getNum() << "\n";
				s.close();
			}
		}
	}
	LOG(LogLevel::Debug) << "Output " << n_tests << " test ships.";
#endif
}


std::tuple<double, double, double> V2Country::getNationalValueScores() 
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

	return make_tuple(libertyScore, equalityScore, orderScore);
}


void V2Country::addRelation(V2::Relation& newRelation)
{
	relations.insert(std::make_pair(newRelation.getTarget(), newRelation));
}


void V2Country::absorbVassal(V2Country* vassal)
{
	// change province ownership and add owner cores if needed
	map<int, V2Province*> vassalProvinces = vassal->getProvinces();
	for (auto provItr = vassalProvinces.begin(); provItr != vassalProvinces.end(); provItr++)
	{
		provItr->second->setOwner(tag);
		provItr->second->setController(tag);
		provItr->second->addCore(tag);
	}
	vassal->provinces.clear();

	// accept cultures from the vassal
	if (primaryCulture != vassal->getPrimaryCulture())
	{
		acceptedCultures.insert(vassal->getPrimaryCulture());
	}
	set<string> cultures = vassal->getAcceptedCultures();
	for (auto itr: cultures)
	{
		if (primaryCulture != itr)
		{
			acceptedCultures.insert(itr);
		}
	}

	// take vassal's armies
	srcCountry->takeArmies(vassal->getSourceCountry());

	// assume the vassal's decisions (just canals, at the moment)
	for (const auto& decision : vassal->decisions)
	{
		decisions.push_back(decision);
	}
	vassal->decisions.clear();
}


void V2Country::setColonyOverlord(V2Country* colony)
{
	colonyOverlord = colony;
}


V2Country* V2Country::getColonyOverlord()
{
	return colonyOverlord;
}


std::string	V2Country::getColonialRegion()
{
	return srcCountry->getColonialRegion();
}


static bool FactoryCandidateSortPredicate(const pair<double, V2State*>& lhs, const pair<double, V2State*>& rhs)
{
	if (lhs.first != rhs.first)
		return lhs.first > rhs.first;
	return lhs.second->getID() < rhs.second->getID();
}


bool V2Country::addFactory(const V2::Factory& factory)
{
	// check factory techs
	std::string requiredTech = factory.getRequiredTech();
	if (requiredTech != "")
	{
		vector<string>::iterator itr = find(techs.begin(), techs.end(), requiredTech);
		if (itr == techs.end())
		{
			return false;
		}
	}

	// check factory inventions
	if ((theConfiguration.getVic2Gametype() == "vanilla") || (theConfiguration.getVic2Gametype() == "AHD"))
	{
		if (inventions.count(factory.getRequiredInvention()) != 0)
		{
			return false;
		}
	}

	// find a state to add the factory to, which meets the factory's requirements
	vector<pair<double, V2State*>> candidates;
	for (vector<V2State*>::iterator itr = states.begin(); itr != states.end(); ++itr)
	{
		if ( (*itr)->isColonial() )
		{
			continue;
		}

		if ( (*itr)->getFactoryCount() >= 8 )
		{
			continue;
		}

		if (factory.requiresCoastal())
		{
			if ( !(*itr)->isCoastal() )
				continue;
		}

		if ( !(*itr)->hasLandConnection() )
		{
			continue;
		}

		double candidateScore	 = (*itr)->getSuppliedInputs(factory) * 100;
		candidateScore				-= static_cast<double>((*itr)->getFactoryCount()) * 10;
		candidateScore				+= (*itr)->getManuRatio();
		candidates.push_back(pair<double, V2State*>(candidateScore, (*itr) ));
	}

	sort(candidates.begin(), candidates.end(), FactoryCandidateSortPredicate);

	if (candidates.size() == 0)
	{
		return false;
	}

	V2State* target = candidates[0].second;
	target->addFactory(factory);
	numFactories++;
	return true;
}


void V2Country::addRailroadtoCapitalState()
{
	for (vector<V2State*>::iterator i = states.begin(); i != states.end(); i++)
	{
		if ( (*i)->provInState(capital) )
		{
			(*i)->addRailroads();
		}
	}
}


void V2Country::convertUncivReforms(int techGroupAlgorithm, double topTech, int topInstitutions, const mappers::TechGroupsMapper& techGroupsMapper)
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
	}
}

void V2Country::oldCivConversionMethod() // civilisation level conversion method for games up to 1.18
{
	if (srcCountry == nullptr) return;

	if ((srcCountry->getTechGroup() == "western") || (srcCountry->getTechGroup() == "high_american") || (srcCountry->getTechGroup() == "eastern") || (srcCountry->getTechGroup() == "ottoman") || (srcCountry->numEmbracedInstitutions() >= 7))//civilised, do nothing
	{
		civilized = true;
	}
	else
	{
		civilized = false;
	}
	if ((srcCountry->getTechGroup() == "western") || (srcCountry->getTechGroup() == "high_american") || (srcCountry->getTechGroup() == "eastern") || (srcCountry->getTechGroup() == "ottoman") || (srcCountry->numEmbracedInstitutions() >= 7))
	{
		// civilized, do nothing
	}
	else if (((srcCountry->getTechGroup() == "north_american") || (srcCountry->getTechGroup() == "mesoamerican") ||	(srcCountry->getTechGroup() == "south_american") || (srcCountry->getTechGroup() == "new_world") || (srcCountry->getTechGroup() == "andean")) && (srcCountry->numEmbracedInstitutions() <= 3))
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << " and " << srcCountry->numEmbracedInstitutions() << " institutions. westernization at 0%";
		uncivReforms = new V2::UncivReforms(0, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else if ((srcCountry->getIsolationism() == 0) && (srcCountry->numEmbracedInstitutions() >= 6))
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << ", " << srcCountry->numEmbracedInstitutions() << " institutions and an isolationism of " << srcCountry->numEmbracedInstitutions() << ". westernization at 50%";
		uncivReforms = new V2::UncivReforms(50, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else if ((srcCountry->getTechGroup() == "muslim") || (srcCountry->numEmbracedInstitutions() >= 6))
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << " and " << srcCountry->numEmbracedInstitutions() << " institutions. westernization at 44%";
		uncivReforms = new V2::UncivReforms(44, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else if ((srcCountry->getTechGroup() == "indian") || (srcCountry->getIsolationism() == 0))
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << ", " << srcCountry->numEmbracedInstitutions() << " institutions and an isolationism of " << srcCountry->numEmbracedInstitutions() << ".  Westernization at 40%";
		uncivReforms = new V2::UncivReforms(40, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else if ((srcCountry->getTechGroup() == "chinese") || (srcCountry->numEmbracedInstitutions() == 5))
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << " and " << srcCountry->numEmbracedInstitutions() << " institutions. westernization at 36%";
		uncivReforms = new V2::UncivReforms(36, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else if (srcCountry->getTechGroup() == "nomad_group")
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << " and " << srcCountry->numEmbracedInstitutions() << " institutions. westernization at 30%";
		uncivReforms = new V2::UncivReforms(30, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else if ((srcCountry->getTechGroup() == "sub_saharan") || (srcCountry->getTechGroup() == "central_african") || (srcCountry->getTechGroup() == "east_african") || (srcCountry->numEmbracedInstitutions() == 4))
	{
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		LOG(LogLevel::Debug) << "Setting unciv reforms for " << tag << " who has tech group " << srcCountry->getTechGroup() << " and " << srcCountry->numEmbracedInstitutions() << " institutions. westernization at 20%";
		uncivReforms = new V2::UncivReforms(20, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
	else
	{
		LOG(LogLevel::Warning) << "Unhandled tech group (" << srcCountry->getTechGroup() << " with " << srcCountry->numEmbracedInstitutions() << " institutions) for " << tag << " - giving no reforms";
		double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech();
		double militaryDev = srcCountry->getMilTech() / totalTechs;
		double socioEconDev = srcCountry->getAdmTech() / totalTechs;
		uncivReforms = new V2::UncivReforms(0, militaryDev, socioEconDev, this);
		government = "absolute_monarchy";
	}
}

void V2Country::newCivConversionMethod(double topTech, int topInsitutions, const mappers::TechGroupsMapper& techGroupsMapper) // civilisation level conversion method for games after 1.18
{
	{
		if (srcCountry != nullptr) {

			double totalTechs = srcCountry->getMilTech() + srcCountry->getAdmTech() + srcCountry->getDipTech();

			// set civilisation cut off for 6 techs behind the the tech leader (30 years behind tech)
			// set number for civilisation level based on techs and institutions
			// at 31 techs behind completely unciv
			// each institution behind is equivalent to 2 techs behind

			double civLevel = (totalTechs + 31 - topTech) * 4;
			civLevel = civLevel + (static_cast<double>(srcCountry->numEmbracedInstitutions()) - topInsitutions) * 8;
			if (civLevel > 100) civLevel = 100;
			if (civLevel < 0) civLevel = 0;

			string techGroup = srcCountry->getTechGroup();

			if (theConfiguration.getEuroCentrism() == Configuration::EUROCENTRISM::EuroCentric)
			{
				literacy *= (1 + (static_cast<double>(techGroupsMapper.getLiteracyFromTechGroup(techGroup)) - 5.0) * 10.0 / 100.0);
				civLevel = civLevel * (static_cast<double>(techGroupsMapper.getWesternizationFromTechGroup(techGroup)) / 10.0);
			}

			literacy = literacy * theConfiguration.getMaxLiteracy() * (pow(10, (civLevel / 100) * 0.9 + 0.1) / 10);

			if (civLevel == 100)
			{
				civilized = true;
			}
			else
			{
				civilized = false;
			}

			if (civilized == false)
			{
				totalTechs = totalTechs - srcCountry->getDipTech();
				double militaryDev = srcCountry->getMilTech() / totalTechs;
				double socioEconDev = srcCountry->getAdmTech() / totalTechs;
				uncivReforms = new V2::UncivReforms((int)(civLevel + 0.5), militaryDev, socioEconDev, this);
				government = "absolute_monarchy";
			}
		}
	}
}

void V2Country::convertLandlessReforms(V2Country* capOwner)
{
	if (capOwner->isCivilized())
	{
		civilized = true;
	}
	else
	{
		civilized = false;
		uncivReforms = capOwner->getUncivReforms();
	}
}


void V2Country::setupPops(
	double popWeightRatio,
	int popConversionAlgorithm,
	const std::map<std::string, std::shared_ptr<EU4::Country>>& theEU4Countries,
	const mappers::ProvinceMapper& provinceMapper
) {
	if (states.size() < 1) // skip entirely for empty nations
		return;

	// create the pops
	for (auto itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		itr->second->doCreatePops(popWeightRatio, this, popConversionAlgorithm, theEU4Countries, provinceMapper);
	}

	// output statistics on pops
	/*map<string, long int> popsData;
	for (auto provItr = provinces.begin(); provItr != provinces.end(); provItr++)
	{
		auto pops = provItr->second->getPops();
		for (auto popsItr = pops.begin(); popsItr != pops.end(); popsItr++)
		{
			auto popItr = popsData.find( (*popsItr)->getType() );
			if (popItr == popsData.end())
			{
				long int newPopSize = 0;
				pair<map<string, long int>::iterator, bool> newIterator = popsData.insert(make_pair((*popsItr)->getType(), 0));
				popItr = newIterator.first;
			}
			popItr->second += (*popsItr)->getSize();
		}
	}
	long int totalPops = 0;
	for (auto dataItr = popsData.begin(); dataItr != popsData.end(); dataItr++)
	{
		totalPops += dataItr->second;
	}

	for (auto dataItr = popsData.begin(); dataItr != popsData.end(); dataItr++)
	{
		double popsPercent = static_cast<double>(dataItr->second) / totalPops;
		string filename = dataItr->first;
		filename += ".csv";
		FILE* dataFile = fopen(filename.c_str(), "a");
		if (dataFile != nullptr)
		{
			fprintf(dataFile, "%s,%d,%f\n", tag.c_str(), dataItr->second, popsPercent);
			fclose(dataFile);
		}
	}*/
}


void V2Country::setArmyTech(double normalizedScore)
{
	if ((theConfiguration.getVic2Gametype() != "vanilla") && !civilized)
		return;

	if (normalizedScore >= -1.0)
	{
		techs.push_back("flintlock_rifles");
		inventions.insert("flintlock_rifle_armament");
	}
	if (normalizedScore >= -0.9)
	{
		techs.push_back("bronze_muzzle_loaded_artillery");
	}
	if (normalizedScore >= -0.2)
	{
		techs.push_back("post_napoleonic_thought");
		inventions.insert("post_napoleonic_army_doctrine");
	}
	if (normalizedScore >= 0.2)
	{
		techs.push_back("army_command_principle");
	}
	if (normalizedScore >= 0.6)
	{
		techs.push_back("military_staff_system");
		inventions.insert("cuirassier_activation");
		inventions.insert("dragoon_activation");
		inventions.insert("_hussar_activation");
	}
	if (normalizedScore >= 1.0)
	{
		techs.push_back("army_professionalism");
		inventions.insert("army_academic_training");
		inventions.insert("field_training");
		inventions.insert("army_societal_status");
	}
}


void V2Country::setNavyTech(double normalizedScore)
{
	if ((theConfiguration.getVic2Gametype() != "vanilla") && !civilized)
		return;

	if (normalizedScore >= 0)
	{
		techs.push_back("post_nelsonian_thought");
		inventions.insert("long_range_fire_tactic");
		inventions.insert("speedy_maneuvering_tactic");
	}
	if (normalizedScore >= 0.036)
	{
		techs.push_back("the_command_principle");
	}
	if (normalizedScore >= 0.571)
	{
		techs.push_back("clipper_design");
		techs.push_back("naval_design_bureaus");
		techs.push_back("alphabetic_flag_signaling");
		inventions.insert("building_station_shipyards");
	}
	if (normalizedScore >= 0.857)
	{
		techs.push_back("battleship_column_doctrine");
		techs.push_back("steamers");
		inventions.insert("long_range_fire_tactic");
		inventions.insert("speedy_maneuvering_tactic");
		inventions.insert("mechanized_fishing_vessels");
		inventions.insert("steamer_automatic_construction_plants");
		inventions.insert("steamer_transports");
		inventions.insert("commerce_raiders");
	}
	if (normalizedScore >= 1.0)
	{
		techs.push_back("naval_professionalism");
		inventions.insert("academic_training");
		inventions.insert("combat_station_training");
		inventions.insert("societal_status");
	}
}


void V2Country::setCommerceTech(double normalizedScore)
{
	if ((theConfiguration.getVic2Gametype() != "vanilla") && !civilized)
		return;

	techs.push_back("no_standard");
	if (normalizedScore >= -0.777)
	{
		techs.push_back("guild_based_production");
	}
	if (normalizedScore >= -0.555)
	{
		techs.push_back("private_banks");
	}
	if (normalizedScore >= -0.333)
	{
		techs.push_back("early_classical_theory_and_critique");
	}
	if (normalizedScore >= -.277)
	{
		techs.push_back("freedom_of_trade");
		inventions.insert("john_ramsay_mcculloch");
		inventions.insert("nassau_william_sr");
		inventions.insert("james_mill");
	}
	if (normalizedScore >= 0.333)
	{
		techs.push_back("stock_exchange");
		inventions.insert("multitude_of_financial_instruments");
		inventions.insert("insurance_companies");
		inventions.insert("regulated_buying_and_selling_of_stocks");
	}
	if (normalizedScore >= 0.777)
	{
		techs.push_back("ad_hoc_money_bill_printing");
		techs.push_back("market_structure");
		inventions.insert("silver_standard");
		inventions.insert("decimal_monetary_system");
		inventions.insert("polypoly_structure");
		inventions.insert("oligopoly_structure");
		inventions.insert("monopoly_structure");
	}

	if (normalizedScore >= 1.0)
	{
		techs.push_back("late_classical_theory");
		inventions.insert("john_elliot_cairnes");
		inventions.insert("robert_torrens");
		inventions.insert("john_stuart_mill");
	}
}


void V2Country::setIndustryTech(double normalizedScore)
{
	if ((theConfiguration.getVic2Gametype() != "vanilla") && !civilized)
		return;

	if (normalizedScore >= -1.0)
	{
		techs.push_back("water_wheel_power");
		inventions.insert("tulls_seed_drill");
	}
	if (normalizedScore >= -0.714)
	{
		techs.push_back("publishing_industry");
	}
	if (normalizedScore >= -0.143)
	{
		techs.push_back("mechanized_mining");
		techs.push_back("basic_chemistry");
		inventions.insert("ammunition_production");
		inventions.insert("small_arms_production");
		inventions.insert("explosives_production");
		inventions.insert("artillery_production");
	}
	if (normalizedScore >= 0.143)
	{
		techs.push_back("practical_steam_engine");
		inventions.insert("rotherham_plough");
	}
	if (normalizedScore >= 0.428)
	{
		techs.push_back("experimental_railroad");
	}
	if (normalizedScore >= 0.714)
	{
		techs.push_back("mechanical_production");
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
		techs.push_back("clean_coal");
		inventions.insert("pit_coal");
		inventions.insert("coke");
	}
}


void V2Country::setCultureTech(double normalizedScore)
{
	if ((theConfiguration.getVic2Gametype() != "vanilla") && !civilized)
		return;

	techs.push_back("classicism_n_early_romanticism");
	inventions.insert("carlism");
	techs.push_back("late_enlightenment_philosophy");
	inventions.insert("declaration_of_the_rights_of_man");
	if (normalizedScore >= -0.333)
	{
		techs.push_back("enlightenment_thought");
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
		techs.push_back("malthusian_thought");
	}
	if (normalizedScore >= 0.333)
	{
		techs.push_back("introspectionism");
	}
	if (normalizedScore >= 0.666)
	{
		techs.push_back("romanticism");
		inventions.insert("romanticist_literature");
		inventions.insert("romanticist_art");
		inventions.insert("romanticist_music");
	}
}

string V2Country::getLocalName()
{
	return localisation.getLocalName();
}


V2::Relation& V2Country::getRelation(const std::string& target)
{
	const auto& relation = relations.find(target);
	if (relation != relations.end()) return relation->second;
	V2::Relation newRelation(target);
	relations.insert(std::make_pair(target, newRelation));
	const auto& newRelRef = relations.find(target);
	return newRelRef->second;
}


void V2Country::addLoan(string creditor, double size, double interest)
{
	map<string, V2Creditor*>::iterator itr = creditors.find(creditor);
	if (itr != creditors.end())
	{
			itr->second->addLoan(size, interest);
	}
	else
	{
		V2Creditor* cred = new V2Creditor(creditor);
		cred->addLoan(size, interest);
		creditors.insert(make_pair(creditor, cred));
	}
}




// find the army most in need of a regiment of this category
V2::Army* V2Country::getArmyForRemainder(V2::REGIMENTTYPE chosenType)
{
	V2::Army* retval = nullptr;
	double retvalRemainder = -1000.0;
	for (auto& army: armies)
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



