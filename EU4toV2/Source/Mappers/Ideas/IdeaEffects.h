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



#ifndef IDEA_EFFECTS_H
#define IDEA_EFFECTS_H



#include "newParser.h"
#include <set>



namespace mappers
{

class IdeaEffects: commonItems::parser
{
	public:
		IdeaEffects(std::istream& theStream);

		int getArmy() const { return army; }
		int getNavy() const { return navy; }
		int getCommerce() const { return commerce; }
		int getCulture() const { return culture; }
		int getIndustry() const { return industry; }

		int getOrder() const { return order; }
		int getLiberty() const { return liberty; }
		int getEquality() const { return equality; }
		int getLiteracy() const { return literacy; }

		int getReactionary() const { return reactionary; }
		int getLiberal() const { return liberal; }

		int getSlavery() const { return slavery; }
		int getUpper_house_composition() const { return upper_house_composition;  }
		int getVote_franchise() const { return vote_franchise; }
		int getVoting_system() const { return voting_system; }
		int getPublic_meetings() const {	return public_meetings; }
		int getPress_rights() const {	return press_rights;	}
		int getTrade_unions() const {	return trade_unions;	}
		int getPolitical_parties() const { return political_parties; }

	private:
		// Values range 0-10. 5 is default.
		int slavery = 5;
		int upper_house_composition = 5;
		int vote_franchise = 5;
		int voting_system = 5;
		int public_meetings = 5;
		int press_rights = 5;
		int trade_unions = 5;
		int political_parties = 5;

		int liberty = 5;
		int equality = 5;
		int order = 5;
		int literacy = 5;

		int reactionary = 5;
		int liberal = 5;

		int army = 5;
		int navy = 5;
		int commerce = 5;
		int culture = 5;
		int industry = 5;
};

}



#endif // IDEA_EFFECTS_H