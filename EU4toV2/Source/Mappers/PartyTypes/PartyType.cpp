#include "ParserHelpers.h"
#include "PartyType.h"

mappers::PartyType::PartyType(std::istream& theStream)
{
	registerKeyword("name", [this](const std::string& language, std::istream& theStream) 
		{
			const commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword("ideology", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString ideoStr(theStream);
			ideology = ideoStr.getString();
		});
	registerKeyword("start_date", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString dateStr(theStream);
			start_date = date(dateStr.getString());
		});
	registerKeyword("end_date", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString dateStr(theStream);
			end_date = date(dateStr.getString());
		});
	registerKeyword("economic_policy", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString economicStr(theStream);
			economic_policy = economicStr.getString();
		});
	registerKeyword("trade_policy", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString tradeStr(theStream);
			trade_policy = tradeStr.getString();
		});
	registerKeyword("religious_policy", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString religiousStr(theStream);
			religious_policy = religiousStr.getString();
		});
	registerKeyword("citizenship_policy", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString citizenshipStr(theStream);
			citizenship_policy = citizenshipStr.getString();
		});
	registerKeyword("war_policy", [this](const std::string& language, std::istream& theStream)
		{
			const commonItems::singleString warStr(theStream);
			war_policy = warStr.getString();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}
