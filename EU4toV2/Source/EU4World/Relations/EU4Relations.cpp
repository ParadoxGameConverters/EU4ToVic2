#include "EU4Relations.h"
#include "ParserHelpers.h"

EU4::EU4Relations::EU4Relations(std::istream& theStream)
{
	registerRegex("[A-Z0-9]{3}", [this](const std::string& tag, std::istream& theStream)
		{
			EU4RelationDetails newDetails(theStream);
			relations.insert(std::make_pair(tag, newDetails));
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
}
