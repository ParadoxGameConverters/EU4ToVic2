#include "EU4WarDetails.h"
#include "ParserHelpers.h"

void EU4::WarDetails::addDetails(std::istream& theStream)
{
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString typeStr(theStream);
			warGoalType = typeStr.getString();
		});
	registerKeyword("province", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt provinceInt(theStream);
			targetProvinceID = provinceInt.getInt();
		});
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString tagStr(theStream);
			targetTag = tagStr.getString();
		});
	registerKeyword("casus_belli", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString cbStr(theStream);
			EU4casusBelli = cbStr.getString();
		});
	registerRegex("\\d+\\.\\d+\\.\\d+", [this](const std::string& dateString, std::istream& theStream) 
		{
			if (startDate == date("1.1.1")) startDate = date(dateString);
			commonItems::ignoreItem(dateString, theStream);			
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}


