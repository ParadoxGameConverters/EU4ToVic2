#include "PartyName.h"
#include "Log.h"

mappers::PartyName::PartyName(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& language, std::istream& theStream) {
		commonItems::singleString incomingStr(theStream);
		languageToNameMap.insert(make_pair(language, incomingStr.getString()));
	});

	parseStream(theStream);
}

std::optional<std::string> mappers::PartyName::getLanguageToName(const std::string& language) const
{
	auto mapping = languageToNameMap.find(language);
	if (mapping != languageToNameMap.end())
	{
		return mapping->second;
	}
	else
	{
		return "";
	}
}