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



#include "ReligionGroup.h"
#include "ParserHelpers.h"



EU4::ReligionGroup::ReligionGroup(const std::string& groupName, std::istream& theStream)
{
	registerKeyword(std::regex("defender_of_faith"), commonItems::ignoreItem);
	registerKeyword(std::regex("can_form_personal_unions"), commonItems::ignoreItem);
	registerKeyword(std::regex("center_of_religion"), commonItems::ignoreItem);
	registerKeyword(std::regex("flags_with_emblem_percentage"), commonItems::ignoreItem);
	registerKeyword(std::regex("flag_emblem_index_range"), commonItems::ignoreItem);
	registerKeyword(std::regex("ai_will_propagate_through_trade"), commonItems::ignoreItem);
	registerKeyword(std::regex("religious_schools"), commonItems::ignoreItem);
	registerKeyword(std::regex("harmonized_modifier"), commonItems::ignoreItem);
	registerKeyword(std::regex("crusade_name"), commonItems::ignoreItem);

	registerKeyword(std::regex("[a-zA-Z0-9_]+"),
		[this, groupName](const std::string& religionName, std::istream& theStream)
	{
		Religion newReligion(groupName, religionName);
		religions.insert(std::make_pair(religionName, newReligion));
		commonItems::ignoreItem(religionName, theStream);
	});

	parseStream(theStream);
}