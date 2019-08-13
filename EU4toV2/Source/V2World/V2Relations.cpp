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



#include "V2Relations.h"
#include "../EU4World/EU4Relations.h"



V2Relations::V2Relations(string newTag)
{
	tag					= newTag;
	value					= 0;
	militaryAccess		= false;
	lastSendDiplomat	= date();
	lastWar				= date();
	level					= 2; // Neutral
}


V2Relations::V2Relations(string newTag, EU4Relations* oldRelations)
{
	tag					= newTag;
	value					= oldRelations->getRelations();
	militaryAccess		= oldRelations->hasMilitaryAccess();
	lastSendDiplomat	= oldRelations->getDiplomatLastSent();
	lastWar				= oldRelations->getLastWar();
	level					= 2; // Neutral
}


std::ostream& operator<<(std::ostream& output, const V2Relations& relations)
{
	output << "\t" << relations.tag << "=\n";
	output << "\t{\n";
	output << "\t\tvalue=" << relations.value << "\n";
	if (relations.militaryAccess)
	{
		output << "\t\tmilitary_access=yes\n";
	}
	output << "\t\tlevel=" << relations.level << "\n";
	output << "\t}\n";

	return output;
}


void V2Relations::setLevel(int lvl)
{
	if (lvl < 0 || lvl > 5)
	{
		return;
	}
	level = lvl;
}
