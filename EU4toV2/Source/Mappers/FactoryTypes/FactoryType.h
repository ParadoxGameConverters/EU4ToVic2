#ifndef FACTORY_TYPE_H
#define FACTORY_TYPE_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class FactoryType : commonItems::parser
	{
	public:
		FactoryType() = default;
		FactoryType(std::istream& theStream);
		bool getCoastal() const { return coastal; }
		std::string getRequiredTech() const { return requiredTech; }
		std::string getRequiredInvention() const { return requiredInvention; }
		std::map<std::string, double> getInputs() const { return inputs; }
		std::string getOutputs() const { return outputs; }
		int getStartingCount() const { return startingCount; }
		void setStartingCount(int stcount) { startingCount = stcount; }
		void setFactoryTypeName(std::string name) { factoryTypeName = name; }
		std::string getName() const { return factoryTypeName; }

		void assignTechsAndCounts(const std::map<std::string, std::string>& factoryTechMap, const std::map<std::string, std::string>& factoryInventionMap, const std::map<std::string, int>& startingCounts);

	private:
		std::string factoryTypeName;
		bool coastal = false;
		std::string requiredTech;
		std::string requiredInvention;
		std::map<std::string, double> inputs;
		std::string outputs;
		int startingCount = 0;
	};
}

#endif // FACTORY_TYPE_H