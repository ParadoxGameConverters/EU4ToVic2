#include <ostream>
#include "../Army/Army.h"

std::ostream& V2::operator<<(std::ostream& output, const Army& army)
{
	if (army.regiments.size() == 0)
	{
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
