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
		}
	);
	registerKeyword(std::regex("type"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString typeString(theStream);
			type = typeString.getString();
		}
	);
	registerKeyword(std::regex("female"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString unusedString(theStream);
			female = true;
		}
	);
	registerKeyword(std::regex("manuever"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			manuever = theValue.getInt();
		}
	);
	registerKeyword(std::regex("fire"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			fire = theValue.getInt();
		}
	);
	registerKeyword(std::regex("shock"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			shock = theValue.getInt();
		}
	);
	registerKeyword(std::regex("siege"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt theValue(theStream);
			siege = theValue.getInt();
		}
	);
	registerKeyword(std::regex("country"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString countryString(theStream);
			country = countryString.getString();
		}
	);
	registerKeyword(std::regex("activation"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString dateString(theStream);
			date anActivationDate(dateString.getString());
			activationDate = anActivationDate;
		}
	);
	registerKeyword(std::regex("monarch_id"), [this](const std::string& idType, std::istream& theStream)
		{
			// Ignoring this
			EU4::ID theID(theStream);
		}
	);
	registerKeyword(std::regex("id"), [this](const std::string& idType, std::istream& theStream)
		{
			EU4::ID theID(theStream);
			leaderID = theID.getIDNum();
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreString);

	parseStream(theStream);
}


bool EU4::Leader::isLand() const
{
	if (type == "general" || type == "conquistador")
	{
		return true;
	}
	else if (type == "explorer" || type == "admiral")
	{
		return false;
	}
	else
	{
		LOG(LogLevel::Warning) << "Unknown leader type " << type;
		return false;
	}
}
