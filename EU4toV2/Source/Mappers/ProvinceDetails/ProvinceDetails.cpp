#include "ProvinceDetails.h"
#include "ParserHelpers.h"
#include "Log.h"

mappers::ProvinceDetails::ProvinceDetails(const std::string& filename)
{
	Log(LogLevel::Info) << "Parsing black and white ports.";
	registerKeys();
	parseFile(filename);
	clearRegisteredKeywords();
}

mappers::ProvinceDetails::ProvinceDetails(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ProvinceDetails::registerKeys()
{
	registerKeyword("owner", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ownerStr(theStream);
			owner = ownerStr.getString();
		});
	registerKeyword("controller", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString controllerStr(theStream);
			controller = controllerStr.getString();
		});
	registerKeyword("add_core", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString coreStr(theStream);
			cores.insert(coreStr.getString());
		});
	registerKeyword("trade_goods", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString rgoTypeStr(theStream);
			rgoType = rgoTypeStr.getString();
		});
	registerKeyword("life_rating", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt lifeRatingInt(theStream);
			lifeRating = lifeRatingInt.getInt();
		});
	registerKeyword("terrain", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString terrainStr(theStream);
			terrain = terrainStr.getString();
		});
	registerKeyword("colonial", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colonialInt(theStream);
			colonial = colonialInt.getInt();
		});
	registerKeyword("colony", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt colonyInt(theStream);
			colonyLevel = colonyInt.getInt();
		});
	registerKeyword("naval_base", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt navalBaseLevelInt(theStream);
			navalBaseLevel = navalBaseLevelInt.getInt();
		});
	registerKeyword("fort", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt fortLevelInt(theStream);
			fortLevel = fortLevelInt.getInt();
		});
	registerKeyword("railroad", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt railLevelInt(theStream);
			railLevel = railLevelInt.getInt();
		});
	registerKeyword("is_slave", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString ignoredStr(theStream);
			slaveState = true;
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
