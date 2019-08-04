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



#include "V2Leader.h"
#include "V2LeaderTraits.h"
#include "V2Country.h"
#include "../EU4World/EU4Leader.h"



V2Leader::V2Leader(const EU4::leader* oldLeader, const V2LeaderTraits& traits)
{
	name = oldLeader->getName();
	activationDate	= oldLeader->getActivationDate();
	isLand = oldLeader->isLand();
	personality = traits.getPersonality(oldLeader->getFire(), oldLeader->getShock(), oldLeader->getManuever(), oldLeader->getSiege());
	background = traits.getBackground(oldLeader->getFire(), oldLeader->getShock(), oldLeader->getManuever(), oldLeader->getSiege());
}


std::ostream& operator<<(std::ostream& output, const V2Leader& leader)
{
	output << "leader = {\n";
	output << "\tname=\"" << leader.name << "\"\n";
	output << "\tdate=\"" << leader.activationDate << "\"\n";
	if (leader.isLand)
	{
		output << "\ttype=land\n";
	}
	else
	{
		output << "\ttype=sea\n";
	}
	output << "\tpersonality=\"" << leader.personality << "\"\n";
	output << "\tbackground=\"" << leader.background << "\"\n";
	output << "}\n";
	output << "\n";

	return output;
}
