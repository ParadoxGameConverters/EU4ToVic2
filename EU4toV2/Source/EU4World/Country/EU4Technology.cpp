#include "EU4Technology.h"
#include "ParserHelpers.h"

EU4::EU4Technology::EU4Technology(std::istream& theStream)
{
	registerKeyword("adm_tech", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt techInt(theStream);
			adm = techInt.getInt();
		});
	registerKeyword("dip_tech", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt techInt(theStream);
			dip = techInt.getInt();
		});
	registerKeyword("mil_tech", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt techInt(theStream);
			mil = techInt.getInt();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
}
