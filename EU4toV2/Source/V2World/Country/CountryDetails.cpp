#include "CountryDetails.h"
#include "ParserHelpers.h"
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"

V2::CountryDetails::CountryDetails(const std::string& _filename): filename(_filename)
{
	registerKeys();

	if (Utils::DoesFileExist("./blankMod/output/common/countries/" + filename))
	{
		parseFile("./blankMod/output/common/countries/" + filename);
	}
	else if (Utils::DoesFileExist(theConfiguration.getVic2Path() + "/common/countries/" + filename))
	{
		parseFile(theConfiguration.getVic2Path() + "/common/countries/" + filename);
	}
	// Maybe we're initing a dead nation. If so look in the /other/ place.
	else if (Utils::DoesFileExist("./blankMod/output/history/countries/" + filename))
	{
		parseFile("./blankMod/output/history/countries/" + filename);
	}
	else if (Utils::DoesFileExist(theConfiguration.getVic2Path() + "/history/countries/" + filename))
	{
		parseFile(theConfiguration.getVic2Path() + "/history/countries/" + filename);
	}
	else
	{
		LOG(LogLevel::Debug) << "Could not find file common/countries/" << filename << " - skipping";
	}
	clearRegisteredKeywords();
}

V2::CountryDetails::CountryDetails(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void V2::CountryDetails::registerKeys()
{
	registerKeyword(std::regex("party"), [this](const std::string& unused, std::istream& theStream)
		{
			mappers::PartyType newPartyType(theStream);
			Party newParty(newPartyType);
			parties.push_back(newParty);
		});
	registerKeyword("primary_culture", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString primCulStr(theStream);
			primaryCulture = primCulStr.getString();
		});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString religionStr(theStream);
			religion = religionStr.getString();
		});
	registerKeyword("government", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString governmentStr(theStream);
			government = governmentStr.getString();
		});
	registerKeyword("civilized", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::ignoreItem(unused, theStream);
			civilized = true;
		});
	registerKeyword(std::regex("is_releasable_vassal"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString releasableStr(theStream);
			isReleasableVassal = releasableStr.getString() == "yes";
		});
	registerKeyword(std::regex("nationalvalue"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nationalvalueStr(theStream);
			nationalValue = nationalvalueStr.getString();
		});
	registerKeyword(std::regex("capital"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt capitalInt(theStream);
			capital = capitalInt.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);
}
