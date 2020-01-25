#include "EU4Country.h"
#include "EU4GovernmentSection.h"
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "../Relations/EU4Relations.h"
#include "../History/CountryHistory.h"
#include "../../V2World/Localisation/Localisation.h"
#include "EU4Technology.h"
#include "EU4CountryFlags.h"
#include "EU4Modifier.h"
#include "EU4ActiveIdeas.h"

EU4::Country::Country(
	const std::string& countryTag, 
	const EU4::Version& theVersion, 
	std::istream& theStream, 
	const mappers::IdeaEffectMapper& 
	ideaEffectMapper, 
	const mappers::CultureGroups& cultureGroupsMapper
): tag(countryTag)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theName(theStream);
			name = theName.getString();
		});
	registerKeyword("custom_name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theName(theStream);
			randomName = V2::Localisation::convert(theName.getString());
			customNation = true;
		});
	registerKeyword("adjective", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theAdjective(theStream);
			adjective = theAdjective.getString();
		});
	// This is obsolete and not applicable from at least 1.19+, probably further back
	registerKeyword("map_color", [this](const std::string& unused, std::istream& theStream)
		{
			auto colorColor = commonItems::Color(theStream);
			colorColor.RandomlyFlunctuate(30);
			// Countries whose colors are included in the object here tend to be generated countries,
			// i.e. colonial nations which take on the color of their parent. To help distinguish 
			// these countries from their parent's other colonies we randomly adjust the color.
			nationalColors.setMapColor(colorColor);
		});
	registerKeyword("colors", [this](const std::string& colorsString, std::istream& theStream)
		{
			NationalSymbol theSection(theStream);
			nationalColors = theSection;
		});
	registerKeyword("capital", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theCapital(theStream);
			capital = theCapital.getInt();
		});
	registerKeyword("technology_group", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theTechGroup(theStream);
			techGroup = theTechGroup.getString();
		});
	registerKeyword("liberty_desire", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble theLibertyDesire(theStream);
			libertyDesire = theLibertyDesire.getDouble();
		});
	registerKeyword("institutions", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::intList theInstitutions(theStream);
			for (auto institution: theInstitutions.getInts())
			{
				if (institution == 1)
				{
					embracedInstitutions.push_back(true);
				}
				else
				{
					embracedInstitutions.push_back(false);
				}
			}
		});
	registerKeyword("isolationism", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt isolationismValue(theStream);
			isolationism = isolationismValue.getInt();
		});
	registerKeyword("primary_culture", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString thePrimaryCulture(theStream);
			primaryCulture = thePrimaryCulture.getString();
		});
	registerKeyword("accepted_culture", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theAcceptedCulture(theStream);
			acceptedCultures.push_back(theAcceptedCulture.getString());
		});
	registerKeyword("government_rank", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theGovernmentRank(theStream);
			governmentRank = theGovernmentRank.getInt();
		});
	registerKeyword("realm_development", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theDevelopment(theStream);
			development = theDevelopment.getInt();
		});
	// obsolete since 1.18 at the latest
	registerKeyword("culture_group_union", [this, theVersion, cultureGroupsMapper](const std::string& unused, std::istream& theStream)
		{
			if (theVersion < EU4::Version("1.7.0.0"))
			{
				commonItems::singleString cultureGroup(theStream);
				culturalUnion = cultureGroupsMapper.getCulturalGroup(cultureGroup.getString());
			}
			else
			{
				mappers::CultureGroup newUnion(tag + "_union", theStream);
				culturalUnion = newUnion;
			}
		});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theReligion(theStream);
			religion = theReligion.getString();
		});
	// Obsolete since 1.26.0
	registerKeyword("score", [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleDouble theScore(theStream);
			score = theScore.getDouble();
		});
	//Relevant since 1.20 but we only use it for 1.26+
	registerKeyword("age_score", [this, theVersion](const std::string& unused, std::istream& theStream) 
		{
			if (theVersion >= EU4::Version("1.26.0.0")) 
			{
				commonItems::doubleList ageScores(theStream);
				for (auto& agScore : ageScores.getDoubles()) score += agScore;
			}
		});
	registerKeyword("stability", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble theStability(theStream);
			stability = theStability.getDouble();
		});
	registerKeyword("technology", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4Technology techBlock(theStream);
			admTech = techBlock.getAdm();
			dipTech = techBlock.getDip();
			milTech = techBlock.getMil();
		});
	registerRegex("flags|hidden_flags|variables", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4CountryFlags flagsBlock(theStream);
			for (const auto& flag : flagsBlock.getFlags()) flags[flag] = true;
		});
	registerKeyword("modifier", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4Modifier newModifier(theStream);
			if (newModifier.getModifier().size() > 0)
			{
				modifiers[newModifier.getModifier()] = true;
			}
		});
	registerKeyword("government", [this, theVersion](const std::string& unused, std::istream& theStream)
		{
			if (theVersion < EU4::Version("1.23.0.0"))
			{
				commonItems::singleString govStr(theStream);
				government = govStr.getString();
			}
			else
			{
				EU4::GovernmentSection theSection(theStream);
				government = theSection.getGovernment();
				governmentReforms = theSection.getGovernmentReforms();
			}
		});
	registerKeyword("active_relations", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4Relations activeRelations(theStream);
			relations = activeRelations.getRelations();
		});
	registerRegex("army|navy", [this](const std::string& armyFloats, std::istream& theStream)
		{
			EU4Army army(theStream, armyFloats);
			armies.push_back(army);
		});
	registerKeyword("active_idea_groups", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::EU4ActiveIdeas activeIdeas(theStream);
			nationalIdeas = activeIdeas.getActiveIdeas();
		});
	registerRegex("legitimacy|horde_unity|devotion|meritocracy|republican_tradition", [this](const std::string& legitimacyType, std::istream& theStream)
		{
			commonItems::singleDouble theLegitimacy(theStream);
			if (legitimacyType == "legitimacy" || legitimacyType == "meritocracy") legitimacy = theLegitimacy.getDouble();
			if (legitimacyType == "horde_unity") hordeUnity = theLegitimacy.getDouble();
			if (legitimacyType == "devotion") devotion = theLegitimacy.getDouble();
			if (legitimacyType == "republican_tradition") republicanTradition = theLegitimacy.getDouble();
		});
	registerKeyword("average_autonomy", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble autonomyDbl(theStream);
			averageAutonomy = autonomyDbl.getDouble();
		});
	registerKeyword("parent", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString alsoUnused(theStream);
			colony = true;
		});
	registerKeyword("colonial_parent", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString alsoUnused(theStream);
			colony = true;
		});
	registerKeyword("overlord", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString theOverlord(theStream);
			overlord = theOverlord.getString();
		});
	// This is obsolete and not applicable from at least 1.19+, probably further back:
	// In current savegame implementation, custom_colors stores a color triplet, but apparently it used to
	// store a custom colors block with flag and symbol - which is now in colors block.
	registerKeyword("country_colors", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::CustomColors colorBlock(theStream);
			nationalColors.setCustomColors(colorBlock);
			nationalColors.setCustomColorsInitialized();
		});
	// This is obsolete and not applicable from at least 1.19+, probably further back
	registerKeyword("revolutionary_colors", [this](const std::string& unused, std::istream& theStream)
		{
			auto colorColor = commonItems::Color(theStream);
			nationalColors.setRevolutionaryColor(colorColor);
		});
	registerKeyword("history", [this](const std::string& unused, std::istream& theStream)
		{
			CountryHistory theCountryHistory(theStream);
			historicalLeaders = theCountryHistory.getLeaders();
			if (!theCountryHistory.getDynasty().empty()) historicalEntry.lastDynasty = theCountryHistory.getDynasty();
		});
	registerKeyword("leader", [this](const std::string& unused, std::istream& theStream)
		{
			ID idBlock(theStream);
			activeLeaderIDs.insert(idBlock.getIDNum());
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();

	determineJapaneseRelations();
	determineInvestments(ideaEffectMapper);
	determineLibertyDesire();
	determineCulturalUnion(cultureGroupsMapper);
	filterLeaders();
	// finalize history data.
	if (government == "republic" || government == "theocracy") historicalEntry.monarchy = false;
}

void EU4::Country::filterLeaders()
{
	for (const auto& leader : historicalLeaders)
	{
		// Dropping all leaders not currently in service (regardless of asignment).
		if (activeLeaderIDs.find(leader.getID()) != activeLeaderIDs.end())
		{
			militaryLeaders.push_back(leader);
		}
	}
}

void EU4::Country::dropMinorityCultures()
{
	std::vector<std::string> updatedCultures;
	for (std::string acceptedCulture: acceptedCultures)
	{
		double culturalDevelopment = 0;
		for (auto& p : provinces)
		{
			culturalDevelopment += p->getCulturePercent(acceptedCulture) * p->getTotalDevModifier();
		}
		if ((culturalDevelopment / development) > 0.15)
		{
			updatedCultures.push_back(acceptedCulture);
		}
	}
	acceptedCultures = updatedCultures;
}

double EU4::Country::getLegitimacy() const
{
	if (government == "republic") return republicanTradition;
	if (government == "theocracy") return devotion;
	if (government == "monarchy") return legitimacy;
	if (government == "tribe") return hordeUnity;
	return 50;
}


void EU4::Country::determineCulturalUnion(const mappers::CultureGroups& cultureGroupsMapper)
{
	if ((development >= 1000) || (governmentRank > 2))
	{
		culturalUnion = cultureGroupsMapper.getCulturalGroup(primaryCulture);
	}
}

void EU4::Country::determineJapaneseRelations()
{
	// pre-dharma
	if (government == "daimyo") possibleDaimyo = true;
	if (government == "shogunate") possibleShogun = true;

	// post-dharma
	if (governmentReforms.count("shogunate")) possibleShogun = true;
	if (governmentReforms.count("indep_daimyo")) possibleDaimyo = true;
	if (governmentReforms.count("daimyo")) possibleDaimyo = true;
}

void EU4::Country::determineInvestments(const mappers::IdeaEffectMapper& ideaEffectMapper)
{
	int ignoredMechanics = 0;
	if (nationalIdeas.size() > 1) // There is always the default "TAG_ideas" inside, which we ignore for now.
	{
		// We need to set reforms at -5 because there is a national idea inside that will give us 5 and we cannot filter it as it's unordered.
		armyInvestment = -5.0;
		commerceInvestment = -5.0;
		cultureInvestment = -5.0;
		industryInvestment = -5.0;
		navyInvestment = -5.0;

		slaveryInvestment = -5.0;
		upper_house_compositionInvestment = -5.0;
		vote_franchiseInvestment = -5.0;
		voting_systemInvestment = -5.0;
		public_meetingsInvestment = -5.0;
		press_rightsInvestment = -5.0;
		trade_unionsInvestment = -5.0;
		political_partiesInvestment = -5.0;

		libertyInvestment = -5.0;
		equalityInvestment = -5.0;
		orderInvestment = -5.0;
		literacyInvestment = -5.0;

		reactionaryInvestment = -5.0;
		liberalInvestment = -5.0;

		for (auto idea : nationalIdeas)
		{
			armyInvestment += ideaEffectMapper.getArmyFromIdea(idea);
			commerceInvestment += ideaEffectMapper.getCommerceFromIdea(idea);
			cultureInvestment += ideaEffectMapper.getCultureFromIdea(idea);
			industryInvestment += ideaEffectMapper.getIndustryFromIdea(idea);
			navyInvestment += ideaEffectMapper.getNavyFromIdea(idea);

			slaveryInvestment += ideaEffectMapper.getSlaveryFromIdea(idea);
			upper_house_compositionInvestment += ideaEffectMapper.getUpper_house_compositionFromIdea(idea);
			vote_franchiseInvestment += ideaEffectMapper.getVote_franchiseFromIdea(idea);
			voting_systemInvestment += ideaEffectMapper.getVoting_systemFromIdea(idea);
			public_meetingsInvestment += ideaEffectMapper.getPublic_meetingsFromIdea(idea);
			press_rightsInvestment += ideaEffectMapper.getPress_rightsFromIdea(idea);
			trade_unionsInvestment += ideaEffectMapper.getTrade_unionsFromIdea(idea);
			political_partiesInvestment += ideaEffectMapper.getPolitical_partiesFromIdea(idea);

			libertyInvestment += ideaEffectMapper.getLibertyFromIdea(idea);
			equalityInvestment += ideaEffectMapper.getEqualityFromIdea(idea);
			orderInvestment += ideaEffectMapper.getOrderFromIdea(idea);
			literacyInvestment += ideaEffectMapper.getLiteracyFromIdea(idea);
 
			reactionaryInvestment += ideaEffectMapper.getReactionaryFromIdea(idea);
			liberalInvestment += ideaEffectMapper.getLiberalFromIdea(idea);
		}
	}

	for (auto reformStr : governmentReforms)
	{
		if (reformStr.find("_mechanic") == std::string::npos) //ignore the basic legacy mechanics, focus on actual reforms
		{
			armyInvestment += ideaEffectMapper.getArmyFromIdea(reformStr);
			commerceInvestment += ideaEffectMapper.getCommerceFromIdea(reformStr);
			cultureInvestment += ideaEffectMapper.getCultureFromIdea(reformStr);
			industryInvestment += ideaEffectMapper.getIndustryFromIdea(reformStr);
			navyInvestment += ideaEffectMapper.getNavyFromIdea(reformStr);

			slaveryInvestment += ideaEffectMapper.getSlaveryFromIdea(reformStr);
			upper_house_compositionInvestment += ideaEffectMapper.getUpper_house_compositionFromIdea(reformStr);
			vote_franchiseInvestment += ideaEffectMapper.getVote_franchiseFromIdea(reformStr);
			voting_systemInvestment += ideaEffectMapper.getVoting_systemFromIdea(reformStr);
			public_meetingsInvestment += ideaEffectMapper.getPublic_meetingsFromIdea(reformStr);
			press_rightsInvestment += ideaEffectMapper.getPress_rightsFromIdea(reformStr);
			trade_unionsInvestment += ideaEffectMapper.getTrade_unionsFromIdea(reformStr);
			political_partiesInvestment += ideaEffectMapper.getPolitical_partiesFromIdea(reformStr);

			libertyInvestment += ideaEffectMapper.getLibertyFromIdea(reformStr);
			equalityInvestment += ideaEffectMapper.getEqualityFromIdea(reformStr);
			orderInvestment += ideaEffectMapper.getOrderFromIdea(reformStr);
			literacyInvestment += ideaEffectMapper.getLiteracyFromIdea(reformStr);

			reactionaryInvestment += ideaEffectMapper.getReactionaryFromIdea(reformStr);
			liberalInvestment += ideaEffectMapper.getLiberalFromIdea(reformStr);
		}
		else
		{
			ignoredMechanics += 1;
		}
	}

	// We actually need the averages, to play against government and reforms.

	int nideasSize = static_cast<int>(nationalIdeas.size());
	int govRefsSize = static_cast<int>(governmentReforms.size());
	int totalDivider = nideasSize - 1 + govRefsSize - ignoredMechanics;

	armyInvestment /= totalDivider;
	commerceInvestment /= totalDivider;
	cultureInvestment /= totalDivider;
	industryInvestment /= totalDivider;
	navyInvestment /= totalDivider;

	slaveryInvestment /= totalDivider;
	upper_house_compositionInvestment /= totalDivider;
	vote_franchiseInvestment /= totalDivider;
	voting_systemInvestment /= totalDivider;
	public_meetingsInvestment /= totalDivider;
	press_rightsInvestment /= totalDivider;
	trade_unionsInvestment /= totalDivider;
	political_partiesInvestment /= totalDivider;

	libertyInvestment /= totalDivider;
	equalityInvestment /= totalDivider;
	orderInvestment /= totalDivider;
	literacyInvestment /= totalDivider;

	reactionaryInvestment /= totalDivider;
	liberalInvestment /= totalDivider;
}

void EU4::Country::determineLibertyDesire()
{
	if ((colony) && (libertyDesire != 0.0))
	{
		auto relationship = relations.find(overlord);
		if (relationship != relations.end())
		{
			std::string attitude = relationship->second.getAttitude();
			if (attitude == "attitude_rebellious")
			{
				libertyDesire = 95.0;
			}
			else if (attitude == "attitude_disloyal")
			{
				libertyDesire = 90.0;
			}
			else if (attitude == "attitude_disloyal_vassal")	// for pre-1.14 games
			{
				libertyDesire = 90.0;
			}
			else if (attitude == "attitude_outraged")
			{
				libertyDesire = 85.0;
			}
			else if (attitude == "attitude_rivalry")
			{
				libertyDesire = 80.0;
			}
			else if (attitude == "attitude_hostile")
			{
				libertyDesire = 75.0;
			}
			else if (attitude == "attitude_threatened")
			{
				libertyDesire = 65.0;
			}
			else if (attitude == "attitude_neutral")
			{
				libertyDesire = 50.0;
			}
			else if (attitude == "attitude_defensive")
			{
				libertyDesire = 35.0;
			}
			else if (attitude == "attitude_domineering")
			{
				libertyDesire = 20.0;
			}
			else if (attitude == "attitude_protective")
			{
				libertyDesire = 15.0;
			}
			else if (attitude == "attitude_allied")
			{
				libertyDesire = 10.0;
			}
			else if (attitude == "attitude_friendly")
			{
				libertyDesire = 10.0;
			}
			else if (attitude == "attitude_loyal")
			{
				libertyDesire = 5.0;
			}
			else if (attitude == "attitude_overlord")
			{
				libertyDesire = 5.0;
			}
			else if (attitude == "attitude_vassal")	// for pre-1.14 games
			{
				libertyDesire = 5.0;
			}
			else
			{
				LOG(LogLevel::Warning) << "Unknown attitude type " << attitude << " while setting liberty desire for " << tag;
				libertyDesire = 95.0;
			}
		}
	}
}

void EU4::Country::readFromCommonCountry(const std::string& fileName, const std::string& fullFilename)
{
	if (name.empty())
	{
		// For this country's name we will use the stem of the file name.
		size_t extPos = fileName.find_last_of('.');
		name = fileName.substr(0, extPos);
	}

	if (!nationalColors.getMapColor())
	{
		registerKeyword("color", [this](const std::string& unused, std::istream& theStream)
			{
				auto color = commonItems::Color(theStream);
				nationalColors.setMapColor(color);
			}
		);
		registerRegex("[a-zA-Z0-9_]+", commonItems::ignoreItem);

		parseFile(fullFilename);
	}
}

void EU4::Country::setLocalisationName(const std::string& language, const std::string& name)
{
	if (name.size() == 1)
	{
		std::string newName = name + name;
		namesByLanguage[language] = newName;
	}
	else
	{
		namesByLanguage[language] = name;
	}
}

void EU4::Country::setLocalisationAdjective(const std::string& language, const std::string& adjective)
{
	adjectivesByLanguage[language] = adjective;
}

void EU4::Country::addProvince(const std::shared_ptr<Province>& province)
{
	provinces.push_back(province);
}

void EU4::Country::addCore(const std::shared_ptr<Province>& core)
{
	cores.push_back(core);
}

bool EU4::Country::hasModifier(std::string modifier) const
{
	std::map<std::string, bool>::const_iterator itr = modifiers.find(modifier);
	return (itr != modifiers.end());
}

bool EU4::Country::hasNationalIdea(std::string ni) const
{
	auto itr = nationalIdeas.find(ni);
	return (itr != nationalIdeas.end());
}

bool EU4::Country::hasFlag(std::string flag) const
{
	std::map<std::string, bool>::const_iterator itr = flags.find(flag);
	return (itr != flags.end());
}

void EU4::Country::resolveRegimentTypes(const mappers::UnitTypeMapper& unitTypeMapper)
{
	for (std::vector<EU4Army>::iterator itr = armies.begin(); itr != armies.end(); ++itr)
	{
		itr->resolveRegimentTypes(unitTypeMapper);
	}
}

int EU4::Country::getManufactoryCount() const
{
	int mfgCount = 0;	// the number of manus
	for (auto province: provinces)
	{
		//TODO: Dump buildings and values into own parser - duplication at V2::Province
		if (province->hasBuilding("weapons")) ++mfgCount;
		if (province->hasBuilding("wharf")) ++mfgCount;
		if (province->hasBuilding("textile")) ++mfgCount;
		if (province->hasBuilding("plantations")) ++mfgCount;
		if (province->hasBuilding("tradecompany")) ++mfgCount;
		if (province->hasBuilding("farm_estate")) ++mfgCount;
		if (province->hasBuilding("mills")) ++mfgCount;
		if (province->hasBuilding("furnace")) mfgCount += 3;
	}
	return mfgCount;
}

void EU4::Country::eatCountry(std::shared_ptr<EU4::Country> target)
{
	// autocannibalism is forbidden
	if (target->getTag() == tag) return;

	LOG(LogLevel::Info) << " - " << tag << " is assimilating " << target->getTag();

	// for calculation of weighted averages
	unsigned int totalProvinces = target->provinces.size() + provinces.size();
	if (totalProvinces == 0)
	{
		totalProvinces = 1;
	}
	const double myWeight = (double)provinces.size() / (double)totalProvinces;						// the amount of influence from the main country
	const double targetWeight = (double)target->provinces.size() / (double)totalProvinces;		// the amount of influence from the target country

	// acquire target's cores (always)
	for (auto& core: target->getCores())
	{
		addCore(core);
		core->addCore(tag);
		core->removeCore(target->tag);
	}

	// everything else, do only if this country actually currently exists
	if (target->provinces.size() > 0)
	{
		// acquire target's provinces
		for (unsigned int j = 0; j < target->provinces.size(); j++)
		{
			target->provinces[j]->setOwnerString(tag);
			target->provinces[j]->setControllerString(tag);
			addProvince(target->provinces[j]);
		}

		// acquire target's armies, navies, admirals, and generals
		armies.insert(armies.end(), target->armies.begin(), target->armies.end());
		militaryLeaders.insert(militaryLeaders.end(), target->militaryLeaders.begin(), target->militaryLeaders.end());

		// rebalance prestige, badboy, inflation and techs from weighted average
		score						= myWeight * score						+ targetWeight * target->score;
		admTech					= myWeight * admTech						+ targetWeight * target->admTech;
		dipTech					= myWeight * dipTech						+ targetWeight * target->dipTech;
		milTech					= myWeight * milTech						+ targetWeight * target->milTech;
		armyInvestment			= myWeight * armyInvestment			+ targetWeight * target->armyInvestment;
		navyInvestment			= myWeight * navyInvestment			+ targetWeight * target->navyInvestment;
		commerceInvestment	= myWeight * commerceInvestment		+ targetWeight * target->commerceInvestment;
		industryInvestment	= myWeight * industryInvestment		+ targetWeight * target->industryInvestment;
		cultureInvestment		= myWeight * cultureInvestment		+ targetWeight * target->cultureInvestment;
	}

	// coreless, landless countries will be cleaned up automatically
	target->clearProvinces();
	target->clearCores();
}

void EU4::Country::takeArmies(std::shared_ptr<Country> target)
{
	// acquire target's armies, navies, admirals, and generals
	armies.insert(armies.end(), target->armies.begin(), target->armies.end());
	militaryLeaders.insert(militaryLeaders.end(), target->militaryLeaders.begin(), target->militaryLeaders.end());
	target->clearArmies();
}

void EU4::Country::clearArmies()
{
	armies.clear();
	militaryLeaders.clear();
}

bool EU4::Country::cultureSurvivesInCores(const std::map<std::string, std::shared_ptr<EU4::Country>>& theCountries)
{
	for (auto core: cores)
	{
		if (core->getOwnerString() == "") continue;
		if (core->getCulturePercent(primaryCulture) >= 0.5) continue;

		auto owner = theCountries.find(core->getOwnerString());
		if ((owner != theCountries.end()) && (owner->second->getPrimaryCulture() != primaryCulture))
		{
			return true;
		}
	}
	return false;
}

std::string EU4::Country::getName(const std::string& language) const
{
	if (!randomName.empty())
	{
		return randomName;
	}

	if (namesByLanguage.empty())
	{
		// We're returning english base name as a default for all languages where we lack localization.
		return name;
	}

	std::map<std::string, std::string>::const_iterator findIter = namesByLanguage.find(language);
	if (findIter != namesByLanguage.end())
	{
		if (findIter->second.empty())
		{
			// Default to english base name for incomplete localization
			return name;
		}
		else 
		{
			return findIter->second;
		}
	}
	else
	{
		return name;
	}
}

std::string EU4::Country::getAdjective(const std::string& language) const
{
	if (!randomName.empty())
	{
		return randomName;
	}

	if (adjectivesByLanguage.empty())
	{
		// For dynamic countries there are no localizations save for the savegame one, 
		// so we return english for all languuages.
		return adjective;
	}

	std::map<std::string, std::string>::const_iterator findIter = adjectivesByLanguage.find(language);
	std::map<std::string, std::string>::const_iterator engIter = adjectivesByLanguage.find("english");

	if (findIter != adjectivesByLanguage.end())
	{
		if (findIter->second.empty())
		{
			// Default to english for incomplete localization
			return engIter->second;
		}
		else
		{
			return findIter->second;
		}
	}
	else
	{
		// We're returning english adjective as a default for all languages where we lack localization.
		return engIter->second;
	}
}

int EU4::Country::numEmbracedInstitutions() const {
	int total = 0;
	for (unsigned int i = 0; i < embracedInstitutions.size(); i++) {
		if (embracedInstitutions[i]) total++;
	}
	return total;
}

void EU4::Country::clearProvinces()
{
	provinces.clear();
}

void EU4::Country::clearCores()
{
	cores.clear();
}

int EU4::Country::getTotalDev() const
{
	int totalDev = 0;
	for (const auto& province : provinces) totalDev += static_cast<int>(province->getTotalDevModifier());
	return totalDev;
}
