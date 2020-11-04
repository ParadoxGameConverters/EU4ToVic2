#include "output.h"

std::ostream& mappers::operator<<(std::ostream& output, const Technology& technology)
{
	output << technology.name << " = {\n";
	output << "\tarea = " << technology.area << "\n";
	output << "\tyear = " << technology.year << "\n";
	output << "\tcost = " << technology.cost << "\n";
	if (!technology.uncivMilitary.empty())
		output << "\tunciv_military = " << technology.uncivMilitary << "\n";

	output << "\n";
	for (const auto& effect: technology.effects)
	{
		output << "\t" << effect << "\n";
	}
	output << "\n";

	output << "\tai_chance " << technology.aiChance << "\n";
	output << "}\n";
	output << "\n";
	return output;
}