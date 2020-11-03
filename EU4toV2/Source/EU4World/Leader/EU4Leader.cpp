#include "EU4Leader.h"
#include "EU4LeaderID.h"
#include "Log.h"
#include "ParserHelpers.h"

EU4::Leader::Leader(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU4::Leader::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream) {
		leaderType = commonItems::singleString(theStream).getString();
	});
	registerKeyword("female", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		female = true;
	});
	registerKeyword("manuever", [this](const std::string& unused, std::istream& theStream) { // don't fix PDX typo!
		maneuver = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("fire", [this](const std::string& unused, std::istream& theStream) {
		fire = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("shock", [this](const std::string& unused, std::istream& theStream) {
		shock = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("siege", [this](const std::string& unused, std::istream& theStream) {
		siege = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("activation", [this](const std::string& unused, std::istream& theStream) {
		activationDate = date(commonItems::singleString(theStream).getString());
	});
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream) {
		const LeaderID theID(theStream);
		leaderID = theID.getIDNum();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

bool EU4::Leader::isLand() const
{
	if (leaderType == "general" || leaderType == "conquistador")
		return true;
	if (leaderType == "explorer" || leaderType == "admiral")
		return false;

	LOG(LogLevel::Warning) << "Unknown leader type " << leaderType;
	return false;
}
