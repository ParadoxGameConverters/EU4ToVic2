#include "Reforms.h"
#include "../V2Country.h"

V2::Reforms::Reforms(const V2::V2Country* dstCountry, const std::shared_ptr<EU4::Country> srcCountry):
	slavery(dstCountry->getSlavery()),
	vote_franchise(dstCountry->getVote_franchise()),
	upper_house_composition(dstCountry->getUpper_house_composition()),
	voting_system(dstCountry->getVoting_system()),
	public_meetings(dstCountry->getPublic_meetings()),
	press_rights(dstCountry->getPress_rights()),
	trade_unions(dstCountry->getTrade_unions()),
	political_parties(dstCountry->getPolitical_parties())
{
	if ((srcCountry->hasModifier("the_abolish_slavery_act")) || (srcCountry->hasModifier("abolish_slavery_act")))
	{
		abolishSlavery = true;
	}
}
