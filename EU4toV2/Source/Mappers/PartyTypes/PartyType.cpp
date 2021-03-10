#include "PartyType.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::PartyType::PartyType(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PartyType::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("ideology", [this](const std::string& unused, std::istream& theStream) {
		ideology = commonItems::singleString(theStream).getString();
	});
	registerKeyword("start_date", [this](const std::string& unused, std::istream& theStream) {
		start_date = date(commonItems::singleString(theStream).getString());
	});
	registerKeyword("end_date", [this](const std::string& unused, std::istream& theStream) {
		end_date = date(commonItems::singleString(theStream).getString());
	});
	registerRegex("[a-z_]+", [this](const std::string& policy, std::istream& theStream) {
		const commonItems::singleString positionStr(theStream);
		policies.insert(std::make_pair(policy, positionStr.getString()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::string> mappers::PartyType::getPolicyPosition(const std::string& policy) const
{
	if (policies.contains(policy))
		return policies.at(policy);
	else
		return std::nullopt;
}