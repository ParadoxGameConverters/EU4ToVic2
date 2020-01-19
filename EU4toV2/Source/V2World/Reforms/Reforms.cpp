#include "Reforms.h"
#include "../Country/Country.h"

V2::Reforms::Reforms(const mappers::CountryDetails& details, const std::shared_ptr<EU4::Country> srcCountry):
	slavery(details.slaveryInvestment),
	vote_franchise(details.vote_franchiseInvestment),
	upper_house_composition(details.upper_house_compositionInvestment),
	voting_system(details.voting_systemInvestment),
	public_meetings(details.public_meetingsInvestment),
	press_rights(details.press_rightsInvestment),
	trade_unions(details.trade_unionsInvestment),
	political_parties(details.political_partiesInvestment)
{
	if (srcCountry->hasModifier("the_abolish_slavery_act") || srcCountry->hasModifier("abolish_slavery_act"))
	{
		abolishSlavery = true;
	}
	initialized = true;
}
