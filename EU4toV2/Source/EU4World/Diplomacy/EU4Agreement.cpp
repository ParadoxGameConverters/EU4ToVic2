#include "EU4Agreement.h"
#include "ParserHelpers.h"

EU4::EU4Agreement::EU4Agreement(std::istream& theStream)
{
	registerRegex("subject_type|type", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleString typeStr(theStream);
			agreementType = typeStr.getString();
		});
	registerKeyword("start_date", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString dateStr(theStream);
			startDate = date(dateStr.getString());
		});
	registerKeyword("end_date", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString dateStr(theStream);
			endDate = date(dateStr.getString());
		});
	registerKeyword("first", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString firstStr(theStream);
			originTag = firstStr.getString();
		});
	registerKeyword("second", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString secondStr(theStream);
			targetTag = secondStr.getString();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}


