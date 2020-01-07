#include "EU4Agreement.h"
#include "ParserHelpers.h"

EU4::EU4Agreement::EU4Agreement(std::istream& theStream)
{
	registerKeyword(std::regex("subject_type|type"), [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString typeStr(theStream);
			agreementType = typeStr.getString();
		}
	);
	registerKeyword(std::regex("start_date"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString dateStr(theStream);
			startDate = date(dateStr.getString());
		}
	);
	registerKeyword(std::regex("end_date"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString dateStr(theStream);
			endDate = date(dateStr.getString());
		}
	);
	registerKeyword(std::regex("first"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString firstStr(theStream);
			originTag = firstStr.getString();
		}
	);
	registerKeyword(std::regex("second"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString secondStr(theStream);
			targetTag = secondStr.getString();
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}


