#include "TaipingCSA.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "Log.h"

V2::TaipingCSA::TaipingCSA(std::istream& effect)
{
	registerKeyword("any_country", [this](const std::string& unused, std::istream& theStream) {
		TaipingCSA newTaipingCSA(theStream);
		countries.insert(make_pair(newTaipingCSA.getTag(), newTaipingCSA.getHistory()));
	});
	registerRegex("[A-Z]{3}", [this](const std::string& tag, std::istream& theStream) {
		TaipingCSA newTaipingCSA(theStream);
		countries.insert(make_pair(tag, newTaipingCSA.getHistory()));
	});
	registerRegex("[A-Z]{3}_[0-9]+|[0-9]+", [this](const std::string& id, std::istream& theStream) {
		TaipingCSA newTaipingCSA(theStream);
		countryCores.insert(make_pair(id, newTaipingCSA.getCore()));
	});
	registerKeyword("limit", [this](const std::string& unused, std::istream& theStream) {
		Limit newLimit(theStream);
		tag = newLimit.getTag();
	});
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& entry, std::istream& theStream) {
		commonItems::singleString historyStr(theStream);
		history.insert(make_pair(entry, historyStr.getString()));
	});
	registerKeyword("add_core", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString tagStr(theStream);
		core = tagStr.getString();
	});	
	registerKeyword("any_owned", commonItems::ignoreItem);
	registerKeyword("any_owned_province", commonItems::ignoreItem);
	registerKeyword("activate_technology", commonItems::ignoreItem);
	registerKeyword("add_country_modifier", commonItems::ignoreItem);
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
	parseStream(effect);
	clearRegisteredKeywords();
}

V2::Limit::Limit(std::istream& theStream)
{
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString tagStr(theStream);
		tag = tagStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
	parseStream(theStream);
	clearRegisteredKeywords();
}