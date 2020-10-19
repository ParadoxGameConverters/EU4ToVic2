#include "Taiping.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "Log.h"

V2::Taiping::Taiping(std::istream& effect)
{
	registerKeyword("any_country", [this](const std::string& unused, std::istream& theStream) {
		const auto body = commonItems::stringOfItem(theStream).getString();
		const auto& fullEffect = "any_country " + body;
		std::stringstream bodyStream(body);
		Taiping newTaiping(bodyStream);
		if (const auto& tag = newTaiping.getTag(); !tag.empty())
			tagEffectMap.insert(make_pair(newTaiping.getTag(), fullEffect));
		else
			nonCountrySpecificEffects.push_back(fullEffect);
	});
	registerRegex("[A-Z]{3}", [this](const std::string& tag, std::istream& theStream) {
		const auto body = commonItems::stringOfItem(theStream).getString();
		const auto& fullEffect = tag + " " + body;
		std::stringstream bodyStream(body);
		Taiping newTaiping(bodyStream);
		tagEffectMap.insert(make_pair(tag, fullEffect));
	});
	registerRegex("[A-Z]{3}_[0-9]+|[0-9]+", [this](const std::string& id, std::istream& theStream) {
		const auto body = commonItems::stringOfItem(theStream).getString();
		const auto& fullEffect = id + " " + body;
		countryCores.push_back(fullEffect);
	});
	registerKeyword("limit", [this](const std::string& unused, std::istream& theStream) {
		Limit newLimit(theStream);
		tag = newLimit.getTag();
	});
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