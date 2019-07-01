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



#include "IdeaEffectMapper.h"
#include "Ideas/IdeaEffects.h"
#include "Log.h"



mappers::IdeaEffectMapper::IdeaEffectMapper(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& idea, std::istream& theStream) {
		IdeaEffects ideaEffects(theStream);

		armyInvestmentIdeas[idea] = ideaEffects.getArmyInvestmentValue();
		navyInvestmentIdeas[idea] = ideaEffects.getNavyInvestmentValue();
		commerceInvestmentIdeas[idea] = ideaEffects.getCommerceInvestmentValue();
		cultureInvestmentIdeas[idea] = ideaEffects.getCultureInvestmentValue();
		industryInvestmentIdeas[idea] = ideaEffects.getIndustryInvestmentValue();

		armyTechIdeas[idea] = ideaEffects.getArmyTechScoreValue();
		navyTechIdeas[idea] = ideaEffects.getNavyTechScoreValue();
		commerceTechIdeas[idea] = ideaEffects.getCommerceTechScoreValue();
		cultureTechIdeas[idea] = ideaEffects.getCultureTechScoreValue();
		industryTechIdeas[idea] = ideaEffects.getIndustryTechScoreValue();

		UHLiberalIdeas[idea] = ideaEffects.getUpperHouseLiberalValue();
		UHReactionaryIdeas[idea] = ideaEffects.getUpperHouseReactionaryValue();

		orderIdeas[idea] = ideaEffects.getOrderInfluenceValue();
		libertyIdeas[idea] = ideaEffects.getLibertyInfluenceValue();
		equalityIdeas[idea] = ideaEffects.getEqualityInfluenceValue();

		literacyIdeas[idea] = ideaEffects.getLiteracyLevels();
	});

	LOG(LogLevel::Info) << "getting idea effects";
	parseStream(theStream);
}


int mappers::IdeaEffectMapper::getArmyInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = armyInvestmentIdeas.find(ideaName);
	if (idea != armyInvestmentIdeas.end())
	{
		return idea->second * ideaLevel;
	}
	else
	{
		return 0;
	}
}


int mappers::IdeaEffectMapper::getCommerceInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = commerceInvestmentIdeas.find(ideaName);
	if (idea != commerceInvestmentIdeas.end())
	{
		return idea->second * ideaLevel;
	}
	else
	{
		return 0;
	}
}


int mappers::IdeaEffectMapper::getCultureInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = cultureInvestmentIdeas.find(ideaName);
	if (idea != cultureInvestmentIdeas.end())
	{
		return idea->second * ideaLevel;
	}
	else
	{
		return 0;
	}
}


int mappers::IdeaEffectMapper::getIndustryInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = industryInvestmentIdeas.find(ideaName);
	if (idea != industryInvestmentIdeas.end())
	{
		return idea->second * ideaLevel;
	}
	else
	{
		return 0;
	}
}


int mappers::IdeaEffectMapper::getNavyInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = navyInvestmentIdeas.find(ideaName);
	if (idea != navyInvestmentIdeas.end())
	{
		return idea->second * ideaLevel;
	}
	else
	{
		return 0;
	}
}


double mappers::IdeaEffectMapper::getUHLiberalFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = UHLiberalIdeas.find(ideaName);
	if (idea != UHLiberalIdeas.end())
	{
		return idea->second * (ideaLevel + 1.0);
	}
	else
	{
		return 0;
	}
}


double mappers::IdeaEffectMapper::getUHReactionaryFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = UHReactionaryIdeas.find(ideaName);
	if (idea != UHReactionaryIdeas.end())
	{
		return idea->second * (ideaLevel + 1.0);
	}
	else
	{
		return 0;
	}
}


double mappers::IdeaEffectMapper::getLiteracyFromIdea(const std::string& ideaName, int ideaLevel) const
{
	double literacy = 0.0;

	auto idea = literacyIdeas.find(ideaName);
	if (idea != literacyIdeas.end())
	{
		for (auto level: idea->second)
		{
			if (ideaLevel >= level)
			{
				literacy += 0.1;
			}
		}
	}

	return literacy;
}


int mappers::IdeaEffectMapper::getOrderInfluenceFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = orderIdeas.find(ideaName);
	if (idea != orderIdeas.end())
	{
		if (ideaLevel == 7)
		{
			ideaLevel++;
		}
		return (ideaLevel + 1) * idea->second;
	}
	else
	{
		return 0;
	}
}


int mappers::IdeaEffectMapper::getLibertyInfluenceFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = libertyIdeas.find(ideaName);
	if (idea != libertyIdeas.end())
	{
		if (ideaLevel == 7)
		{
			ideaLevel++;
		}
		return (ideaLevel + 1) * idea->second;
	}
	else
	{
		return 0;
	}
}


int mappers::IdeaEffectMapper::getEqualityInfluenceFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = equalityIdeas.find(ideaName);
	if (idea != equalityIdeas.end())
	{
		if (ideaLevel == 7)
		{
			ideaLevel++;
		}
		return (ideaLevel + 1) * idea->second;
	}
	else
	{
		return 0;
	}
}


double mappers::IdeaEffectMapper::getArmyTechFromIdeas(const std::map<std::string, int>& ideas) const
{
	double ideaEffect = 0.0;
	for (auto idea: ideas)
	{
		auto techIdea = armyTechIdeas.find(idea.first);
		if (techIdea != armyTechIdeas.end())
		{
			ideaEffect += techIdea->second * idea.second;
		}
	}

	return ideaEffect;
}


double mappers::IdeaEffectMapper::getCommerceTechFromIdeas(const std::map<std::string, int>& ideas) const
{
	double ideaEffect = 0.0;
	for (auto idea: ideas)
	{
		auto techIdea = commerceTechIdeas.find(idea.first);
		if (techIdea != commerceTechIdeas.end())
		{
			ideaEffect += techIdea->second * idea.second;
		}
	}

	return ideaEffect;
}


double mappers::IdeaEffectMapper::getCultureTechFromIdeas(const std::map<std::string, int>& ideas) const
{
	double ideaEffect = 0.0;
	for (auto idea: ideas)
	{
		auto techIdea = cultureTechIdeas.find(idea.first);
		if (techIdea != cultureTechIdeas.end())
		{
			ideaEffect += techIdea->second * idea.second;
		}
	}

	return ideaEffect;
}


double mappers::IdeaEffectMapper::getIndustryTechFromIdeas(const std::map<std::string, int>& ideas) const
{
	double ideaEffect = 0.0;
	for (auto idea: ideas)
	{
		auto techIdea = industryTechIdeas.find(idea.first);
		if (techIdea != industryTechIdeas.end())
		{
			ideaEffect += techIdea->second * idea.second;
		}
	}

	return ideaEffect;
}


double mappers::IdeaEffectMapper::getNavyTechFromIdeas(const std::map<std::string, int>& ideas) const
{
	double ideaEffect = 0.0;
	for (auto idea: ideas)
	{
		auto techIdea = navyTechIdeas.find(idea.first);
		if (techIdea != navyTechIdeas.end())
		{
			ideaEffect += techIdea->second * idea.second;
		}
	}

	return ideaEffect;
}