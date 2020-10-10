#include "ModCommons.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

V2::ModCommons::ModCommons(const std::string& filename)
{
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem colorStr(theStream);
		color = colorStr.getString();
	});
	registerKeyword("graphical_culture", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString graphicalCultureStr(theStream);
		graphicalCulture = graphicalCultureStr.getString();
	});
	registerKeyword("party", [this](const std::string& unused, std::istream& theStream) {
		const mappers::PartyType newPartyType(theStream);
		const Party newParty(newPartyType);
		parties.push_back(newParty);
	});
	registerKeyword("unit_names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringOfItem unitNamesStr(theStream);
		unitNames = unitNamesStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	const auto& file = determineFilePath(filename);
	if (file)
		parseFile(*file);
	clearRegisteredKeywords();

	setPartyDates();
}

std::optional<std::string> V2::ModCommons::determineFilePath(const std::string& filename)
{
	const auto& mod = theConfiguration.getVic2Path();
	if (commonItems::DoesFileExist(mod + "/common/countries/" + filename))
		return mod + "/common/countries/" + filename;
	else
		return std::nullopt;
}

void V2::ModCommons::setPartyDates()
{
	bool conservativeSet = false, liberalSet = false, reactionarySet = false;

	for (auto& party: parties)
	{
		if (party.getIdeology() == "conservative" && !conservativeSet)
		{
			party.setStartDate("1000.1.1");
			conservativeSet = true;
			continue;
		}
		if (party.getIdeology() == "liberal" && !liberalSet)
		{
			party.setStartDate("1680.1.1");
			liberalSet = true;
			continue;
		}
		if (party.getIdeology() == "reactionary" && !reactionarySet)
		{
			party.setStartDate("1790.1.1");
			reactionarySet = true;
		}
	}
}