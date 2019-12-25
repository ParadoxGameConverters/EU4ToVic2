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



#include "ReformMapper.h"
#include "../Helpers/ReformProperties.h"
#include "Log.h"
#include "Object.h"
#include "ParadoxParserUTF8.h"
#include <vector>



ReformMapper* ReformMapper::instance = nullptr;



ReformMapper::ReformMapper()
{
	LOG(LogLevel::Info) << "Parsing Reform mappings";

	std::shared_ptr<Object> ReformMapObj = parser_UTF8::doParseFile("ReformMapping.txt");
	if (ReformMapObj == NULL)
	{
		LOG(LogLevel::Error) << "Could not parse file ReformMapping.txt";
		exit(-1);
	}

	initReformMap(ReformMapObj->getLeaves());
}


void ReformMapper::initReformMap(std::vector<std::shared_ptr<Object>> objs)
{
	for (auto rule: objs)
	{
		processRule(rule);
	}
}


void ReformMapper::processRule(std::shared_ptr<Object> rule)
{
	std::string dstReform;
	ReformProperties reformProperties;

	for (auto item: rule->getLeaves())
	{
		if (item->getKey() == "reform")
		{
			dstReform = item->getLeaf();
		}
		if (item->getKey() == "enforce")
		{
			reformProperties.setEnforce(item->getLeaf());
		}
		if (item->getKey() == "liberty")
		{
			reformProperties.setLiberty(stoi(item->getLeaf()));
		}
		if (item->getKey() == "equality")
		{
			reformProperties.setEquality(stoi(item->getLeaf()));
		}
		if (item->getKey() == "order")
		{
			reformProperties.setOrder(stoi(item->getLeaf()));
		}
		if (item->getKey() == "slavery")
		{
			reformProperties.setSlavery(stoi(item->getLeaf()));
		}
		if (item->getKey() == "upper_house_composition")
		{
			reformProperties.setUpper_house_composition(stoi(item->getLeaf()));
		}
		if (item->getKey() == "vote_franchise")
		{
			reformProperties.setVote_franchise(stoi(item->getLeaf()));
		}
		if (item->getKey() == "voting_system")
		{
			reformProperties.setVoting_system(stoi(item->getLeaf()));
		}
		if (item->getKey() == "public_meetings")
		{
			reformProperties.setPublic_meetings(stoi(item->getLeaf()));
		}
		if (item->getKey() == "press_rights")
		{
			reformProperties.setPress_rights(stoi(item->getLeaf()));
		}
		if (item->getKey() == "trade_unions")
		{
			reformProperties.setTrade_unions(stoi(item->getLeaf()));
		}
		if (item->getKey() == "political_parties")
		{
			reformProperties.setPolitical_parties(stoi(item->getLeaf()));
		}
		if (item->getKey() == "literacy")
		{
			reformProperties.setLiteracy(stoi(item->getLeaf()));
		}
		if (item->getKey() == "army")
		{
			reformProperties.setArmy(stoi(item->getLeaf()));
		}
		if (item->getKey() == "commerce")
		{
			reformProperties.setCommerce(stoi(item->getLeaf()));
		}
		if (item->getKey() == "culture")
		{
			reformProperties.setCulture(stoi(item->getLeaf()));
		}
		if (item->getKey() == "industry")
		{
			reformProperties.setIndustry(stoi(item->getLeaf()));
		}
		if (item->getKey() == "navy")
		{
			reformProperties.setNavy(stoi(item->getLeaf()));
		}
		if (item->getKey() == "reactionary")
		{
			reformProperties.setReactionary(stoi(item->getLeaf()));
		}
		if (item->getKey() == "liberal")
		{
			reformProperties.setLiberal(stoi(item->getLeaf()));
		}
	}

	ReformMap.insert(make_pair(dstReform, reformProperties));
}

