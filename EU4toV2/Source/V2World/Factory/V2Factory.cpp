#include "V2Factory.h"


std::ostream& operator<<(std::ostream& output, const V2Factory& factory)
{
	// V2 takes care of hiring employees on day 1, provided sufficient starting capital
	output << "state_building=\n";
	output << "{\n";
	output << "\tlevel=" << factory.level << "\n";
	output << "\tbuilding = " << factory.factoryType.getName() << "\n";
	output << "\tupgrade = yes\n";
	output << "}\n";

	return output;
}