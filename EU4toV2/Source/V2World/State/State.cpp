#include "State.h"
#include "../Province/Province.h"

V2::State::State(int newId, std::shared_ptr<V2::Province> firstProvince): id(newId)
{
	provinces.push_back(firstProvince);
}

void V2::State::addRailroads()
{
	for (auto province : provinces) province->increaseRailLevel(1);
}

bool V2::State::isCoastal() const
{
	for (auto province: provinces) if (province->isCoastal()) return true;
	return false;
}

bool V2::State::hasLocalSupply(const std::string& product) const
{
	for (auto province: provinces) if (province->getRgoType() == product) return true;
	return false;
}

double V2::State::getSuppliedInputs(std::shared_ptr<Factory> factory) const
{
	// find out the needs
	std::map<std::string, double> inputs = factory->getInputs();
	const int numNeeds = inputs.size();
	if (!numNeeds) return 0;

	// find out what we have from both RGOs and existing factories
	std::map<std::string, double> supplies;
	for (const auto& province: provinces) supplies[province->getRgoType()] += 1.0;
	for (const auto& factoryItr: factories) supplies[factoryItr->getOutputs()] += 1.0;

	// determine how many of the inputs are supplied
	int totalSupplied = 0;
	for (const auto& inputItr: inputs)
	{
		const auto& supplyItr = supplies.find(inputItr.first);
		if (supplyItr != supplies.end()) totalSupplied++;
	}

	return static_cast<double>(totalSupplied) / numNeeds;
}

bool V2::State::provInState(int id) const
{
	for (const auto& province: provinces) if (province->getID() == id) return true;
	return false;
}

void V2::State::addFactory(std::shared_ptr<Factory> factory)
{
	auto addFactory = provinces[0]->addFactory(std::move(factory));
	// Only add factory to the register if it's added to province register.
	if (addFactory) factories.push_back(*addFactory);
}

bool V2::State::hasLandConnection() const
{
	return provinces[0]->hasLandConnection();
}

double V2::State::getMfgRatio() const
{
	// count the manufactories in the source provinces
	double numManus = 0.0;
	for (auto province: provinces) numManus += province->getMfgCount();
	return numManus / provinces.size();
}

void	V2::State::colloectNavalBase()
{
	//Only one naval base in a state
	std::shared_ptr<Province> prov = nullptr;
	int level = 0;
	for (auto province : provinces)
	{
		if (!prov) prov = province;
		level += province->getNavalBaseLevel();
		if (prov->getNavalBaseLevel() < province->getNavalBaseLevel()) prov = province;
	}
	for (auto province : provinces) province->setNavalBaseLevel(0);
	prov->setNavalBaseLevel(level);
}