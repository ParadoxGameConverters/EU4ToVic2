#ifndef FACTORY_H
#define FACTORY_H

#include <ostream>
#include "../../Mappers/FactoryTypes/FactoryType.h"

namespace V2
{
	class Factory
	{
	public:
		Factory() = default;
		explicit Factory(mappers::FactoryType& fType) { factoryType = fType; }
		void increaseLevel() { level++; }

		bool requiresCoastal() const { return factoryType.getCoastal(); }
		std::string getRequiredTech() const { return factoryType.getRequiredTech(); }
		std::string getRequiredInvention() const { return factoryType.getRequiredInvention(); }
		std::string getTypeName() const { return factoryType.getName(); }
		std::map<std::string, double> getInputs() const { return factoryType.getInputs(); }
		std::string getOutputs() const { return factoryType.getOutputs(); };

		friend std::ostream& operator<<(std::ostream& output, const Factory& factory);

	private:
		mappers::FactoryType factoryType;
		int level = 1;
	};
}

#endif // FACTORY_H
