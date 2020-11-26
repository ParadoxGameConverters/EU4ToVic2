#include "CultureGroup.h"
#include "Culture.h"
#include "ParserHelpers.h"

mappers::CultureGroup::CultureGroup(std::string theName, std::istream& theStream): name(std::move(theName))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureGroup::registerKeys()
{
	registerKeyword("unit", [this](const std::string& unused, std::istream& theStream) {
		unit = commonItems::singleString(theStream).getString();
	});
	registerKeyword("leader", [this](const std::string& unused, std::istream& theStream) {
		leader = commonItems::singleString(theStream).getString();
	});
	registerKeyword("is_overseas", [this](const std::string& unused, std::istream& theStream) {
		isOverseas = commonItems::singleString(theStream).getString();
	});
	registerKeyword("union", [this](const std::string& unused, std::istream& theStream) {
		culturalUnionTag = commonItems::singleString(theStream).getString();
	});
	registerRegex(R"([\w_]+)", [this](const std::string& cultureName, std::istream& theStream) {
		auto newCulture = std::make_shared<Culture>(theStream);
		cultures.insert(std::make_pair(cultureName, newCulture));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::CultureGroup::mergeCulture(const std::string& theName, const std::shared_ptr<Culture>& culture)
{
	if (const auto& cultureItr = cultures.find(theName); cultureItr != cultures.end())
	{
		cultureItr->second->addMaleNames(culture->getMaleNames());
		cultureItr->second->addFemaleNames(culture->getFemaleNames());
		cultureItr->second->addDynastyNames(culture->getDynastyNames());
	}
	else
	{
		cultures.insert(std::make_pair(theName, culture));
	}
}

void mappers::CultureGroup::addNeoCulture(const std::string& theName, const std::shared_ptr<Culture>& culture, const std::string& oldCulture)
{
	auto newCulture = std::make_shared<Culture>(*culture);
	newCulture->setNeoCulture(true);
	newCulture->setOriginalCulture(oldCulture);
	cultures.insert(std::make_pair(theName, newCulture));
}
