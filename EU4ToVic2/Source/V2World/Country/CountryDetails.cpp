#include "CountryDetails.h"
#include "../../Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

V2::CountryDetails::CountryDetails(std::filesystem::path _filename): filename(std::move(_filename))
{
	registerKeys();

	if (commonItems::DoesFileExist("blankMod/output/common/countries/" / filename))
	{
		parseFile("blankMod/output/common/countries" / filename);
	}
	else if (commonItems::DoesFileExist(theConfiguration.getVic2Path() / "common/countries" / filename))
	{
		parseFile(theConfiguration.getVic2Path() / "common/countries" / filename);
	}
	// Maybe we're initializing a dead nation. If so look in the /other/ place.
	else if (commonItems::DoesFileExist("blankMod/output/history/countries" / filename))
	{
		parseFile("blankMod/output/history/countries" / filename);
	}
	else if (commonItems::DoesFileExist(theConfiguration.getVic2Path() / "history/countries" / filename))
	{
		parseFile(theConfiguration.getVic2Path() / "history/countries" / filename);
	}
	// what if HPM
	else if (theConfiguration.isHpmEnabled() && commonItems::DoesFileExist("configurables/HPM/common/countries" / filename))
	{
		parseFile("configurables/HPM/common/countries" / filename);
	}
	else
	{
		Log(LogLevel::Debug) << "Could not find file common/countries/" << filename.string() << " - skipping";
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
	registerKeyword("graphical_culture", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString graphCulStr(theStream);
		graphicalCulture = graphCulStr.getString();
	});
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem colorStr(theStream);
		colorString = colorStr.getString();
	});
	registerKeyword("party", [this](const std::string& unused, std::istream& theStream) {
		const mappers::PartyType newPartyType(theStream);
		const Party newParty(newPartyType);
		parties.push_back(newParty);
	});
	registerKeyword("unit_names", [this](const std::string& unused, std::istream& theStream) {
		unitNames = std::make_shared<UnitNames>(theStream);
	});
	registerKeyword("primary_culture", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString primCulStr(theStream);
		primaryCulture = primCulStr.getString();
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionStr(theStream);
		religion = religionStr.getString();
	});
	registerKeyword("government", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString governmentStr(theStream);
		government = governmentStr.getString();
	});
	registerKeyword("civilized", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString civilizedStr(theStream);
		civilized = civilizedStr.getString() == "yes";
	});
	registerKeyword("consciousness", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble conscDbl(theStream);
		consciousness = conscDbl.getDouble();
	});
	registerKeyword("nonstate_consciousness", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble conscDbl(theStream);
		nonstateConsciousness = conscDbl.getDouble();
	});
	registerKeyword("is_releasable_vassal", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString releasableStr(theStream);
		isReleasableVassal = releasableStr.getString() == "yes";
	});
	registerKeyword("nationalvalue", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nationalvalueStr(theStream);
		nationalValue = nationalvalueStr.getString();
	});
	registerKeyword("capital", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt capitalInt(theStream);
		capital = capitalInt.getInt();
	});
	registerKeyword("set_country_flag", [this](std::istream& theStream) {
		const auto& flag = commonItems::getString(theStream);
		if (flag == "new_world_nation")
			countryFlags.emplace("new_world_nation");
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
