/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "IdeaEffects.h"
#include "ParserHelpers.h"



mappers::IdeaEffects::IdeaEffects(std::istream& theStream)
{
	registerKeyword(std::regex("army"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt armyInt(theStream);
		army = armyInt.getInt();
	});
	registerKeyword(std::regex("navy"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt navyInt(theStream);
		navy = navyInt.getInt();
	});
	registerKeyword(std::regex("commerce"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt commerceInt(theStream);
		commerce = commerceInt.getInt();
	});
	registerKeyword(std::regex("culture"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt cultureInt(theStream);
		culture = cultureInt.getInt();
	});
	registerKeyword(std::regex("industry"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt industryInt(theStream);
		industry = industryInt.getInt();
	});

	registerKeyword(std::regex("slavery"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt slaveryInt(theStream);
		slavery = slaveryInt.getInt();
	});
	registerKeyword(std::regex("upper_house_composition"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt upper_house_compositionInt(theStream);
		upper_house_composition = upper_house_compositionInt.getInt();
	});
	registerKeyword(std::regex("vote_franchise"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt vote_franchiseInt(theStream);
		vote_franchise = vote_franchiseInt.getInt();
	});
	registerKeyword(std::regex("voting_system"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt voting_systemInt(theStream);
		voting_system = voting_systemInt.getInt();
	});
	registerKeyword(std::regex("public_meetings"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt public_meetingsInt(theStream);
		public_meetings = public_meetingsInt.getInt();
	});
	registerKeyword(std::regex("press_rights"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt press_rightsInt(theStream);
		press_rights = press_rightsInt.getInt();
	});
	registerKeyword(std::regex("trade_unions"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt trade_unionsInt(theStream);
		trade_unions = trade_unionsInt.getInt();
	});
	registerKeyword(std::regex("trade_unions"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt trade_unionsInt(theStream);
		trade_unions = trade_unionsInt.getInt();
		});
	registerKeyword(std::regex("political_parties"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt political_partiesInt(theStream);
		political_parties = political_partiesInt.getInt();
		});
	registerKeyword(std::regex("reactionary"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt reactionaryInt(theStream);
		reactionary = reactionaryInt.getInt();
		});
	registerKeyword(std::regex("liberal"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt liberalInt(theStream);
		liberal = liberalInt.getInt();
		});

	registerKeyword(std::regex("order"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt orderInt(theStream);
		order = orderInt.getInt();
	});
	registerKeyword(std::regex("liberty"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt libertyInt(theStream);
		liberty = libertyInt.getInt();
	});
	registerKeyword(std::regex("equality"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt equalityInt(theStream);
		equality = equalityInt.getInt();
	});
	registerKeyword(std::regex("literacy"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt literacyInt(theStream);
		literacy = literacyInt.getInt();
	});

	parseStream(theStream);
}