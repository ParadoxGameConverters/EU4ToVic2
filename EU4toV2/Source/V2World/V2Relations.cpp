#include "V2Relations.h"

V2Relations::V2Relations(std::string& newTag)
{
	tag					= newTag;
	value					= 0;
	militaryAccess		= false;
	lastSendDiplomat	= date();
	lastWar				= date();
	level					= 2; // Neutral
}


V2Relations::V2Relations(std::string& newTag, EU4::EU4RelationDetails& oldRelations)
{
	tag					= newTag;
	value					= oldRelations.getRelations();
	militaryAccess		= oldRelations.hasMilitaryAccess();
	lastSendDiplomat	= oldRelations.getDiplomatLastSent();
	lastWar				= oldRelations.getLastWar();
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
