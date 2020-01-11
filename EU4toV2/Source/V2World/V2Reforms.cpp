/*Copyright (c) 2018 The Paradox Game Converters Project

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



#include "V2Reforms.h"
#include "Log.h"
#include "../Configuration.h"
#include "../EU4World/Country/EU4Country.h"
#include "V2Country.h"
#include "V2State.h"
#include "V2Province.h"



V2Reforms::V2Reforms(const V2Country* dstCountry, const std::shared_ptr<EU4::Country> srcCountry)
{
	if ((srcCountry->hasModifier("the_abolish_slavery_act")) || (srcCountry->hasModifier("abolish_slavery_act")))
	{
		abolishSlavery = true;
	}
	slavery = dstCountry->getSlavery();
	upper_house_composition = dstCountry->getUpper_house_composition();
	vote_franchise = dstCountry->getVote_franchise();
	voting_system = dstCountry->getVoting_system();
	public_meetings = dstCountry->getPublic_meetings();
	press_rights = dstCountry->getPress_rights();
	trade_unions = dstCountry->getTrade_unions();
	political_parties = dstCountry->getPolitical_parties();
}


void V2Reforms::output(FILE* output) const
{
	fprintf(output, "\n");
	fprintf(output, "# political reforms\n");
	if (abolishSlavery || (slavery >= 5.0))
	{
		fprintf(output, "slavery=no_slavery\n");
	}
	else
	{
		fprintf(output, "slavery=yes_slavery\n");
	}

	if (vote_franchise >= 9.0)
	{
		fprintf(output, "vote_franschise=universal_voting\n");
	}
	else if (vote_franchise >= 7.5)
	{
		fprintf(output, "vote_franschise=universal_weighted_voting\n");
	}
	else if (vote_franchise >= 5.5)
	{
		fprintf(output, "vote_franschise=wealth_voting\n");
	}
	else if (vote_franchise >= 3.5)
	{
		fprintf(output, "vote_franschise=wealth_weighted_voting\n");
	}
	else if (vote_franchise >= 2.5)
	{
		fprintf(output, "vote_franschise=landed_voting\n");
	}
	else
	{
		fprintf(output, "vote_franschise=none_voting\n");
	}

	if (upper_house_composition >= 7.5)
	{
		fprintf(output, "upper_house_composition=population_equal_weight\n");
	}
	else if (upper_house_composition >= 5.0)
	{
		fprintf(output, "upper_house_composition=state_equal_weight\n");
	}
	else if (upper_house_composition >= 2.5)
	{
		fprintf(output, "upper_house_composition=appointed\n");
	}
	else
	{
		fprintf(output, "upper_house_composition=party_appointed\n");
	}

	if (voting_system >= 6.5)
	{
		fprintf(output, "voting_system=proportional_representation\n");
	}
	else if (voting_system >= 3.0)
	{
		fprintf(output, "voting_system=jefferson_method\n");
	}
	else
	{
		fprintf(output, "voting_system=first_past_the_post\n");
	}

	if (public_meetings >= 5.0)
	{
		fprintf(output, "public_meetings=yes_meeting\n");
	}
	else
	{
		fprintf(output, "public_meetings=no_meeting\n");
	}

	if (press_rights >= 6.5)
	{
		fprintf(output, "press_rights=free_press\n");
	}
	else if (press_rights >= 3.5)
	{
		fprintf(output, "press_rights=censored_press\n");
	}
	else
	{
		fprintf(output, "press_rights=state_press\n");
	}

	if (trade_unions >= 7.5)
	{
		fprintf(output, "trade_unions=all_trade_unions\n");
	}
	else if (trade_unions >= 5.0)
	{
		fprintf(output, "trade_unions=non_socialist\n");
	}
	else if (trade_unions >= 2.5)
	{
		fprintf(output, "trade_unions=state_controlled\n");
	}
	else
	{
		fprintf(output, "trade_unions=no_trade_unions\n");
	}

	if (political_parties >= 8.0)
	{
		fprintf(output, "political_parties=secret_ballots\n");
	}
	else if (political_parties >= 6.0)
	{
		fprintf(output, "political_parties=non_secret_ballots\n");
	}
	else if (political_parties >= 4.0)
	{
		fprintf(output, "political_parties=gerrymandering\n");
	}
	else if (political_parties >= 2.0)
	{
		fprintf(output, "political_parties=harassment\n");
	}
	else
	{
		fprintf(output, "political_parties=underground_parties\n");
	}
}
