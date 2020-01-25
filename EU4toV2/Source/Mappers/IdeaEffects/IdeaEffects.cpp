#include "IdeaEffects.h"
#include "ParserHelpers.h"

mappers::IdeaEffects::IdeaEffects(std::istream& theStream)
{
	registerKeyword("enforce", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleString enforceStr(theStream);
			enforce = enforceStr.getString();
		});
	registerKeyword("army", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt armyInt(theStream);
			army = armyInt.getInt();
		});
	registerKeyword("navy", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt navyInt(theStream);
			navy = navyInt.getInt();
		});
	registerKeyword("commerce", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt commerceInt(theStream);
			commerce = commerceInt.getInt();
		});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt cultureInt(theStream);
			culture = cultureInt.getInt();
		});
	registerKeyword("industry", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt industryInt(theStream);
			industry = industryInt.getInt();
		});
	registerKeyword("slavery", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt slaveryInt(theStream);
			slavery = slaveryInt.getInt();
		});
	registerKeyword("upper_house_composition", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt upper_house_compositionInt(theStream);
			upper_house_composition = upper_house_compositionInt.getInt();
		});
	registerKeyword("vote_franchise", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt vote_franchiseInt(theStream);
			vote_franchise = vote_franchiseInt.getInt();
		});
	registerKeyword("voting_system", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt voting_systemInt(theStream);
			voting_system = voting_systemInt.getInt();
		});
	registerKeyword("public_meetings", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt public_meetingsInt(theStream);
			public_meetings = public_meetingsInt.getInt();
		});
	registerKeyword("press_rights", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt press_rightsInt(theStream);
			press_rights = press_rightsInt.getInt();
		});
	registerKeyword("trade_unions", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt trade_unionsInt(theStream);
			trade_unions = trade_unionsInt.getInt();
		});
	registerKeyword("political_parties", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt political_partiesInt(theStream);
			political_parties = political_partiesInt.getInt();
		});
	registerKeyword("reactionary", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt reactionaryInt(theStream);
			reactionary = reactionaryInt.getInt();
		});
	registerKeyword("liberal", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt liberalInt(theStream);
			liberal = liberalInt.getInt();
		});
	registerKeyword("order", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt orderInt(theStream);
			order = orderInt.getInt();
		});
	registerKeyword("liberty", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt libertyInt(theStream);
			liberty = libertyInt.getInt();
		});
	registerKeyword("equality", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt equalityInt(theStream);
			equality = equalityInt.getInt();
		});
	registerKeyword("literacy", [this](const std::string& unused, std::istream& theStream) 
		{
			const commonItems::singleInt literacyInt(theStream);
			literacy = literacyInt.getInt();
		});

	parseStream(theStream);
	clearRegisteredKeywords();
}