#include "EU4Leader.h"
#include "EU4LeaderID.h"
#include "Log.h"
#include "ParserHelpers.h"

EU4::Leader::Leader(std::istream& theStream)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameString(theStream);
		name = nameString.getString();
	});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString typeString(theStream);
		leaderType = typeString.getString();
	});
	registerKeyword("female", [this](const std::string& unused, std::istream& theStream) {
		commonItems::ignoreItem(unused, theStream);
		female = true;
	});
	registerKeyword("manuever", [this](const std::string& unused, std::istream& theStream) { // don't fix PDX typo!
		const commonItems::singleInt theValue(theStream);
		maneuver = theValue.getInt();
	});
	registerKeyword("fire", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt theValue(theStream);
		fire = theValue.getInt();
	});
	registerKeyword("shock", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt theValue(theStream);
		shock = theValue.getInt();
	});
	registerKeyword("siege", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt theValue(theStream);
		siege = theValue.getInt();
	});
	registerKeyword("country", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString countryString(theStream);
		country = countryString.getString();
	});
	registerKeyword("activation", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dateString(theStream);
		activationDate = date(dateString.getString());
	});
	registerKeyword("id", [this](const std::string& idType, std::istream& theStream) {
		const LeaderID theID(theStream);
		if (theID.getIDNum())
			leaderID = *theID.getIDNum();
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
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
