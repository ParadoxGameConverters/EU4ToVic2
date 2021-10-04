#include "Regiment.h"

V2::Regiment::Regiment(REGIMENTTYPE _regimentType): regimentType(_regimentType)
{
	switch (regimentType)
	{
		case REGIMENTTYPE::irregular:
		case REGIMENTTYPE::infantry:
		case REGIMENTTYPE::cavalry:
		case REGIMENTTYPE::artillery:
			isShip = false;
			break;
		case REGIMENTTYPE::manowar:
		case REGIMENTTYPE::frigate:
		case REGIMENTTYPE::clipper_transport:
			isShip = true;
	}
}

void V2::Regiment::nameShip(std::shared_ptr<UnitNames> unitNames)
{
	switch (regimentType)
	{
		case REGIMENTTYPE::manowar: {
			giveName(unitNames, unitNames->getManowars());
			break;
		}
		case REGIMENTTYPE::frigate: {
			giveName(unitNames, unitNames->getFrigates());
			break;
		}
		case REGIMENTTYPE::clipper_transport: {
			giveName(unitNames, unitNames->getClipperTransports());
		}
	}
}

void V2::Regiment::giveName(std::shared_ptr<UnitNames> unitNames, const std::vector<std::string>& shipTypeNames)
{
	for (const auto& name: shipTypeNames)
	{
		if (unitNames->isNameUsed(name))
		{
			continue;
		}
		setName(name);
		unitNames->addUsedName(name);
		break;
	}
}