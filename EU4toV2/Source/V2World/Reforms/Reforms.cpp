#include "Reforms.h"
#include "../Country/Country.h"

V2::Reforms::Reforms(const CountryDetails& details, const std::shared_ptr<EU4::Country> srcCountry):
	slavery(details.reformInvestments.slaveryInvestment),
	vote_franchise(details.reformInvestments.vote_franchiseInvestment),
	upper_house_composition(details.reformInvestments.upper_house_compositionInvestment),
	voting_system(details.reformInvestments.voting_systemInvestment),
	public_meetings(details.reformInvestments.public_meetingsInvestment),
	press_rights(details.reformInvestments.press_rightsInvestment),
	trade_unions(details.reformInvestments.trade_unionsInvestment),
	political_parties(details.reformInvestments.political_partiesInvestment)
{
	if (srcCountry->hasModifier("the_abolish_slavery_act") || srcCountry->hasModifier("abolish_slavery_act"))
	{
		abolishSlavery = true;
	}
}
