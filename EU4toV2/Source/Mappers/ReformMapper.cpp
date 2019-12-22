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
#include "Log.h"
#include "Object.h"
#include "ParadoxParserUTF8.h"
#include <vector>
using namespace std;



ReformMapper* ReformMapper::instance = nullptr;



ReformMapper::ReformMapper()
{
	LOG(LogLevel::Info) << "Parsing Reform mappings";

	shared_ptr<Object> ReformMapObj = parser_UTF8::doParseFile("ReformMapping.txt");
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


void ReformMapper::processRule(shared_ptr<Object> rule)
{
	string dstReform;
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
			reformProperties.setLiberty(stod(item->getLeaf()));
		}
		if (item->getKey() == "equality")
		{
			reformProperties.setEquality(stod(item->getLeaf()));
		}
		if (item->getKey() == "slavery")
		{
			reformProperties.setSlavery(stod(item->getLeaf()));
		}
		if (item->getKey() == "upper_house_composition")
		{
			reformProperties.setUpper_house_composition(stod(item->getLeaf()));
		}
		if (item->getKey() == "vote_franchise")
		{
			reformProperties.setVote_franchise(stod(item->getLeaf()));
		}
		if (item->getKey() == "voting_system")
		{
			reformProperties.setVoting_system(stod(item->getLeaf()));
		}
		if (item->getKey() == "public_meetings")
		{
			reformProperties.setPublic_meetings(stod(item->getLeaf()));
		}
		if (item->getKey() == "press_rights")
		{
			reformProperties.setPress_rights(stod(item->getLeaf()));
		}
		if (item->getKey() == "trade_unions")
		{
			reformProperties.setTrade_unions(stod(item->getLeaf()));
		}
		if (item->getKey() == "political_parties")
		{
			reformProperties.setPolitical_parties(stod(item->getLeaf()));
		}
		if (item->getKey() == "literarcy")
		{
			reformProperties.setLiterarcy(stod(item->getLeaf()));
		}
		if (item->getKey() == "army")
		{
			reformProperties.setArmy(stod(item->getLeaf()));
		}
		if (item->getKey() == "commerce")
		{
			reformProperties.setCommerce(stod(item->getLeaf()));
		}
		if (item->getKey() == "culture")
		{
			reformProperties.setCulture(stod(item->getLeaf()));
		}
		if (item->getKey() == "industry")
		{
			reformProperties.setIndustry(stod(item->getLeaf()));
		}
		if (item->getKey() == "navy")
		{
			reformProperties.setNavy(stod(item->getLeaf()));
		}
		if (item->getKey() == "reactionary")
		{
			reformProperties.setReactionary(stod(item->getLeaf()));
		}
		if (item->getKey() == "liberal")
		{
			reformProperties.setLiberal(stod(item->getLeaf()));
		}
	}

	ReformMap.insert(make_pair(dstReform, reformProperties));
}



ReformProperties ReformMapper::MatchReform(const string& sourceReform)
{
	auto mapping = ReformMap.find(sourceReform);
	if (mapping != ReformMap.end())
	{
		LOG(LogLevel::Debug) << "Located reform :" << sourceReform;
		return mapping->second;
	}
	else
	{
		LOG(LogLevel::Debug) << "No Reform mapping defined for " << sourceReform;
		return ReformProperties();
	}
}