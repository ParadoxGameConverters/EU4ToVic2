/*Copyright (c) 2014 The Paradox Game Converters Project

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

#include "V2State.h"
#include "Province/Province.h"

V2State::V2State(int newId, std::shared_ptr<V2::Province> firstProvince)
{
	id = newId;
	colonial = false;
	provinces.clear();
	provinces.push_back(firstProvince);

	factories.clear();
}

void V2State::addRailroads()
{
	for (auto itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		(*itr)->setRailLevel(1);
	}
}

bool V2State::isCoastal() const
{
	for (auto itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ((*itr)->isCoastal())
		{
			return true;
		}
	}
	return false;
}

bool V2State::hasLocalSupply(std::string product) const
{
	for (auto itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ((*itr)->getRgoType() == product)
		{
			return true;
		}
	}
	return false;
}

double V2State::getSuppliedInputs(const V2::Factory& factory) const
{
	// find out the needs
	std::map<std::string, double>	inputs = factory.getInputs();
	int						numNeeds = inputs.size();

	// find out what we have from both RGOs and existing factories
	std::map<std::string, double> supplies;
	for (auto itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		std::string rgo = (*itr)->getRgoType();
		supplies[rgo] += 1.0;
	}
	for (std::vector<V2::Factory>::const_iterator itr = factories.begin(); itr != factories.end(); ++itr)
	{
		supplies[itr->getOutputs()] += 1.0;
	}

	// determine how many of the inputs are supplied
	int totalSupplied = 0;
	for (std::map<std::string, double>::const_iterator inputItr = inputs.begin(); inputItr != inputs.end(); ++inputItr)
	{
		std::map<std::string, double>::const_iterator supplyItr = supplies.find(inputItr->first);
		if (supplyItr != supplies.end())
		{
			totalSupplied++;
		}
	}

	return (1.0 * totalSupplied / numNeeds);
}

bool V2State::provInState(int id) const
{
	for (auto itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ((*itr)->getID() == id)
		{
			return true;
		}
	}

	return false;
}

void V2State::addFactory(const V2::Factory& factory)
{
	provinces[0]->addFactory(factory);
	factories.push_back(factory);
}

bool V2State::hasLandConnection() const
{
	if (provinces.size() > 0)
	{
		return provinces[0]->hasLandConnection();
	}
	else
	{
		return false;
	}
}

double V2State::getManuRatio() const
{
	// count the manufactories in the source provinces
	double numManus = 0.0;
	for (auto province: provinces) numManus += province->getMfgCount();

	return numManus / provinces.size();
}

void	V2State::colloectNavalBase()
{
	//Only one naval base in a state
	std::shared_ptr<V2::Province> prov = nullptr;
	int level = 0;
	for (auto province : provinces)
	{
		if (!prov)
		{
			prov = province;
		}
		level += province->getNavalBaseLevel();
		if (prov->getNavalBaseLevel() < province->getNavalBaseLevel())
		{
			prov = province;
		}
	}
	for (auto province : provinces)
	{
		province->setNavalBaseLevel(0);
	}
	prov->setNavalBaseLevel(level > 2 ? 2 : level);
}