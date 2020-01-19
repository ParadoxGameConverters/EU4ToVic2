#include <ostream>
#include "../Diplomacy/Relation.h"

std::ostream& V2::operator<<(std::ostream& output, const Relation& relation)
{
	output << "\t" << relation.target << "=\n";
	output << "\t{\n";
	output << "\t\tvalue=" << relation.value << "\n";
	if (relation.militaryAccess)
	{
		output << "\t\tmilitary_access=yes\n";
	}
	output << "\t\tlevel=" << relation.level << "\n";
	output << "\t}\n";
	return output;
}
