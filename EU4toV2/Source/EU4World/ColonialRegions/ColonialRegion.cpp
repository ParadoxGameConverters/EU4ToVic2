#include "ColonialRegion.h"
#include "ParserHelpers.h"

EU4::ColonialRegion::ColonialRegion(std::istream& theStream)
{
		registerKeyword("provinces", [this](const std::string& unused, std::istream& theStream)
			{
				commonItems::intList theProvinces(theStream);
				for (const auto& provinceID : theProvinces.getInts()) provinces.insert(provinceID);
			});
		registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

		parseStream(theStream);
		clearRegisteredKeywords();
}
