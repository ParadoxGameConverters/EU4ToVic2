#include "output.h"

std::ostream& V2::operator<<(std::ostream& output, const Relation& relation)
{
	output << "" << relation.target << " = {\n";
	output << "\tvalue=" << relation.value << "\n";
	if (relation.militaryAccess)
	{
		output << "\tmilitary_access=yes\n";
	}
	output << "\tlevel=" << relation.level << "\n";
	output << "}\n";
	return output;
}
