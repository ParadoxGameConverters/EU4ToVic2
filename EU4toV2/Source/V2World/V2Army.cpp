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



#include "V2Army.h"
#include "Log.h"
#include <cstring>



static int nextArmyID = 0;


V2ArmyID::V2ArmyID()
{
	type = 40; // seems to be always 40, for army, navy, ship and regiment
	id = ++nextArmyID;
}


void V2ArmyID::output(FILE* out, int indentlevel) const
{
	string indent(indentlevel, '\t');
	fprintf(out, "%sid=\n", indent.c_str());
	fprintf(out, "%s{\n", indent.c_str());
	fprintf(out, "%s\tid=%d\n", indent.c_str(), id);
	fprintf(out, "%s\ttype=%d\n", indent.c_str(), type);
	fprintf(out, "%s}\n", indent.c_str());
}


V2Regiment::V2Regiment(RegimentCategory rc) : category(rc)
{
	name		= "";
	switch (rc)
	{
		case infantry:
			type = "infantry";
			isShip = false;
			break;
		case cavalry:
			type = "cavalry";
			isShip = false;
			break;
		case artillery:
			type = "artillery";
			isShip = false;
			break;
		case heavy_ship:
			type = "manowar";
			isShip = true;
			break;
		case galley:
		case light_ship:
			type = "frigate";
			isShip = true;
			break;
		case transport:
			type = "clipper_transport";
			isShip = true;
			break;
		default:
			LOG(LogLevel::Warning) << "Unexpected regiment category " << rc;
			break;
	}
	home = 0;
}


std::ostream& operator<<(std::ostream& output, const V2Regiment& regiment)
{
	if (regiment.isShip)
	{
		output << "\tship = {\n";
	}
	else
	{
		output << "\tregiment = {\n";
	}
	output << "\t\tname=\"" << regiment.name << "\"\n";
	output << "\t\ttype=" << regiment.type << "\n";
	if (!regiment.isShip)
	{
		output << "\t\thome=" << regiment.home << "\n";
	}
	output << "\t}\n";

	return output;
}


V2Army::V2Army(EU4Army* oldArmy, map<int, int> leaderIDMap)
{
	name			= oldArmy->getName();
	location		= -1;
	regiments.clear();
	memset(armyRemainders, 0, sizeof(armyRemainders));
	sourceArmy	= oldArmy;
	isNavy		= false;
}


std::ostream& operator<<(std::ostream& output, const V2Army& army)
{
	if (army.regiments.size() == 0)
	{
		LOG(LogLevel::Debug) << "Army " << army.name << " has no regiments after conversion; skipping";
		return output;
	}
	if (army.isNavy)
	{
		output << "navy = {\n";
	}
	else
	{
		output << "army = {\n";
	}
	output << "\tname=\"" << army.name << "\"\n";
	output << "\tlocation=" << army.location << "\n";
	for (auto regiment: army.regiments)
	{
		output << regiment;
	}
	output << "}\n";
	output << "\n";

	return output;
}


void V2Army::addRegiment(V2Regiment reg)
{
	regiments.push_back(reg);
}


void V2Army::getRegimentCounts(int counts[num_reg_categories]) const
{
	for (vector<V2Regiment>::const_iterator itr = regiments.begin(); itr != regiments.end(); ++itr)
	{
		counts[itr->getCategory()]++;
	}
}


// used for whitelist/blacklist testing routines
V2Army* V2Army::makeTestNavy(int location)
{
	V2Army* army = new V2Army();
	army->name = "V2 Test Navy";
	army->isNavy = true;
	army->setLocation(location);
	V2Regiment reg(heavy_ship);
	army->addRegiment(reg);
	return army;
}
