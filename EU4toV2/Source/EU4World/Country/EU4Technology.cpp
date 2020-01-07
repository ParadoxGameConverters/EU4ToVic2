#include "EU4Technology.h"
#include "ParserHelpers.h"

EU4::EU4Technology::EU4Technology(std::istream& theStream)
{
	registerKeyword(std::regex("adm_tech"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt techInt(theStream);
			adm = techInt.getInt();
		});
	registerKeyword(std::regex("dip_tech"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt techInt(theStream);
			dip = techInt.getInt();
		});
	registerKeyword(std::regex("mil_tech"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt techInt(theStream);
			mil = techInt.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
