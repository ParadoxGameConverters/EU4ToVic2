#include "EU4Leader.h"
#include "../ID.h"
#include "Log.h"
#include "ParserHelpers.h"

EU4::Leader::Leader(std::istream& theStream)
{
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameString(theStream);
			name = nameString.getString();
		});
	registerKeyword(std::regex("type"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString typeString(theStream);
			leaderType = typeString.getString();
		});
	registerKeyword(std::regex("female"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::ignoreItem(unused, theStream);
			female = true;
		});
	registerKeyword(std::regex("manuever"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			manuever = theValue.getInt();
		});
	registerKeyword(std::regex("fire"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			fire = theValue.getInt();
		});
	registerKeyword(std::regex("shock"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			shock = theValue.getInt();
		});
	registerKeyword(std::regex("siege"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			siege = theValue.getInt();
		});
	registerKeyword(std::regex("country"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString countryString(theStream);
			country = countryString.getString();
		});
	registerKeyword(std::regex("activation"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString dateString(theStream);
			date anActivationDate(dateString.getString());
			activationDate = anActivationDate;
		});
	registerKeyword(std::regex("monarch_id"), commonItems::ignoreItem);
	registerKeyword(std::regex("id"), [this](const std::string& idType, std::istream& theStream)
		{
			EU4::ID theID(theStream);
			leaderID = theID.getIDNum();
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

bool EU4::Leader::isLand() const
{
	if (leaderType == "general" || leaderType == "conquistador") return true;
	if (leaderType == "explorer" || leaderType == "admiral") return false;

	LOG(LogLevel::Warning) << "Unknown leader type " << leaderType;
	return false;
}
