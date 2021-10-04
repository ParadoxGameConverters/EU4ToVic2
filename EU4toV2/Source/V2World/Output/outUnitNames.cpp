#include "../Country/UnitNames.h"
#include "output.h"

std::ostream& V2::operator<<(std::ostream& output, const UnitNames& unitNames)
{
	output << "\n";
	output << "unit_names = {\n";

	output << "\tdreadnought = {\n";
	output << unitNames.outputType(unitNames.dreadnoughts);
	output << "\t}\n";

	output << "\tbattleship = {\n";
	output << unitNames.outputType(unitNames.battleships);
	output << "\t}\n";

	output << "\tironclad = {\n";
	output << unitNames.outputType(unitNames.ironclads);
	output << "\t}\n";

	output << "\tmanowar = {\n";
	output << unitNames.outputType(unitNames.manowars);
	output << "\t}\n";

	output << "\tcruiser = {\n";
	output << unitNames.outputType(unitNames.cruisers);
	output << "\t}\n";

	output << "\tfrigate = {\n";
	output << unitNames.outputType(unitNames.frigates);
	output << "\t}\n";

	output << "\tmonitor = {\n";
	output << unitNames.outputType(unitNames.monitors);
	output << "\t}\n";

	output << "\tclipper_transport = {\n";
	output << unitNames.outputType(unitNames.clipperTransports);
	output << "\t}\n";

	output << "\tsteam_transport = {\n";
	output << unitNames.outputType(unitNames.steamTransports);
	output << "\t}\n";

	output << "\tcommerce_raider = {\n";
	output << unitNames.outputType(unitNames.commerceRaiders);
	output << "\t}\n";

	output << "}";
	return output;
}

std::string V2::UnitNames::outputType(const std::vector<std::string>& shipType) const
{
	bool firstShip = true;
	std::string retString = "\t\t";
	for (auto ship: shipType)
	{
		// wrap inside the double quotes if name contains spaces
		if (std::find_if(ship.begin(), ship.end(), isspace) != ship.end())
		{
			ship = "\"" + ship + "\"";
		}
		// output first ship without the leading space
		if (firstShip)
		{
			retString += ship;
			firstShip = false;
		}
		else
		{
			retString += " " + ship;
		}
	}
	retString += "\n";
	return retString;
}