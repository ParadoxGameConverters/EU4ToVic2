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
#include "IdeaEffects.h"
#include "Log.h"



mappers::IdeaEffectMapper::IdeaEffectMapper(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& idea, std::istream& theStream) {
		IdeaEffects ideaEffects(theStream);

		armyIdeas[idea] = ideaEffects.getArmy();
		navyIdeas[idea] = ideaEffects.getNavy();
		commerceIdeas[idea] = ideaEffects.getCommerce();
		cultureIdeas[idea] = ideaEffects.getCulture();
		industryIdeas[idea] = ideaEffects.getIndustry();

		liberalIdeas[idea] = ideaEffects.getLiberal();
		reactionaryIdeas[idea] = ideaEffects.getReactionary();

		orderIdeas[idea] = ideaEffects.getOrder();
		libertyIdeas[idea] = ideaEffects.getLiberty();
		equalityIdeas[idea] = ideaEffects.getEquality();
		literarcyIdeas[idea] = ideaEffects.getLiteracy();

		slaveryIdeas[idea] = ideaEffects.getSlavery();
		upper_house_compositionIdeas[idea] = ideaEffects.getUpper_house_composition();
		vote_franchiseIdeas[idea] = ideaEffects.getVote_franchise();
		voting_systemIdeas[idea] = ideaEffects.getVoting_system();
		public_meetingsIdeas[idea] = ideaEffects.getPublic_meetings();
		press_rightsIdeas[idea] = ideaEffects.getPress_rights();
		trade_unionsIdeas[idea] = ideaEffects.getTrade_unions();
		political_partiesIdeas[idea] = ideaEffects.getPolitical_parties();


	});

	LOG(LogLevel::Info) << "getting idea effects";
	parseStream(theStream);
}


int mappers::IdeaEffectMapper::getArmyFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = armyIdeas.find(ideaName);
	if ((idea != armyIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getCommerceFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = commerceIdeas.find(ideaName);
	if ((idea != commerceIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getCultureFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = cultureIdeas.find(ideaName);
	if ((idea != cultureIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getIndustryFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = industryIdeas.find(ideaName);
	if ((idea != industryIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getNavyFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = navyIdeas.find(ideaName);
	if ((idea != navyIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getLiberalFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = liberalIdeas.find(ideaName);
	if ((idea != liberalIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getReactionaryFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = reactionaryIdeas.find(ideaName);
	if ((idea != reactionaryIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getLiterarcyFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = literarcyIdeas.find(ideaName);
	if ((idea != literarcyIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getSlaveryFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = slaveryIdeas.find(ideaName);
	if ((idea != slaveryIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getUpper_house_compositionFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = upper_house_compositionIdeas.find(ideaName);
	if ((idea != upper_house_compositionIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getVote_franchiseFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = vote_franchiseIdeas.find(ideaName);
	if ((idea != vote_franchiseIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}

int mappers::IdeaEffectMapper::getVoting_systemFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = voting_systemIdeas.find(ideaName);
	if ((idea != voting_systemIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getPublic_meetingsFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = public_meetingsIdeas.find(ideaName);
	if ((idea != public_meetingsIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getPress_rightsFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = press_rightsIdeas.find(ideaName);
	if ((idea != press_rightsIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getTrade_unionsFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = trade_unionsIdeas.find(ideaName);
	if ((idea != trade_unionsIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}

int mappers::IdeaEffectMapper::getPolitical_partiesFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = political_partiesIdeas.find(ideaName);
	if ((idea != political_partiesIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getOrderFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = orderIdeas.find(ideaName);
	if ((idea != orderIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getLibertyFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = libertyIdeas.find(ideaName);
	if ((idea != libertyIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}


int mappers::IdeaEffectMapper::getEqualityFromIdea(const std::string& ideaName, int ideaLevel) const
{
	auto idea = equalityIdeas.find(ideaName);
	if ((idea != equalityIdeas.end()) && (ideaLevel >= 7))
	{
		return idea->second;
	}
	else
	{
		return 5;
	}
}

