#include "CultureGroup.h"
#include "ParserHelpers.h"

mappers::CultureGroup::CultureGroup(std::string _name, std::istream& theStream):
	name(std::move(_name))
{
	registerKeyword("graphical_culture", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString token(theStream);
			graphicalCulture = token.getString();
		});
	registerKeyword("unit", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString unitStr(theStream);
			unit = unitStr.getString();
		});
	registerKeyword("leader", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString leaderStr(theStream);
			leader = leaderStr.getString();
		});
	registerKeyword("is_overseas", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString overseasStr(theStream);
			isOverseas = overseasStr.getString();
		});
	registerKeyword("union", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString unionStr(theStream);
			culturalUnionTag = unionStr.getString();
		});
	registerKeyword("male_names", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList names(theStream);
			maleNames = names.getStrings();
		});
	registerKeyword("female_names", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList names(theStream);
			femaleNames = names.getStrings();
		});
	registerKeyword("dynasty_names", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::stringList names(theStream);
			dynastyNames = names.getStrings();
		});
	registerRegex("\\w+", [this](const std::string& cultureName, std::istream& theStream)
		{
			Culture newCulture(theStream);
			cultures.insert(make_pair(cultureName, newCulture));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureGroup::mergeCulture(const std::string& name, const Culture& culture)
{
	const auto& cultureItr = cultures.find(name);
	if (cultureItr != cultures.end())
	{
		cultureItr->second.addNameNames(culture.getMaleNames());
		cultureItr->second.addFemaleNames(culture.getFemaleNames());
		cultureItr->second.addDynastyNames(culture.getDynastyNames());		
	}
	else
	{
		cultures.insert(std::make_pair(name, culture));
	}
}

void mappers::CultureGroup::addCulture(const std::string& name, const Culture& culture, const std::string& oldCulture)
{
	auto newCulture = culture;
	newCulture.setNeoCulture(true);
	newCulture.setOriginalCulture(oldCulture);
	cultures.insert(std::make_pair(name, newCulture));
}