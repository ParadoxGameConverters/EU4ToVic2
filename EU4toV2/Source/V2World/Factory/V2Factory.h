#ifndef V2FACTORY_H_
#define V2FACTORY_H_

#include <ostream>
#include "../../Mappers/FactoryTypes/FactoryType.h"

class V2Factory
{
public:
	V2Factory() = default;
	V2Factory(mappers::FactoryType& fType) { factoryType = fType; }
	void increaseLevel() { level++; }

	bool requiresCoastal() const { return factoryType.getCoastal(); }
	std::string getRequiredTech() const { return factoryType.getRequiredTech(); }
	std::string	getRequiredInvention() const { return factoryType.getRequiredInvention(); }
	std::string	getTypeName() const { return factoryType.getName(); }
	std::map<std::string, double> getInputs() const { return factoryType.getInputs(); }
	std::string getOutputs() const { return factoryType.getOutputs(); };

	friend std::ostream& operator<<(std::ostream& output, const V2Factory& factory);

private:
	mappers::FactoryType factoryType;
	int level = 1;
};

#endif // V2FACTORY_H_
