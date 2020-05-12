#ifndef FACTORY_TYPE_H
#define FACTORY_TYPE_H

#include "Parser.h"
#include <map>

namespace mappers
{
	class FactoryType : commonItems::parser
	{
	public:
		FactoryType() = default;
		explicit FactoryType(std::istream& theStream);
		
		void setStartingCount(int stcount) { startingCount = stcount; }
		void setFactoryTypeName(const std::string& name) { factoryTypeName = name; }

		[[nodiscard]] auto getCoastal() const { return coastal; }
		[[nodiscard]] auto getStartingCount() const { return startingCount; }
		[[nodiscard]] const auto& getRequiredTech() const { return requiredTech; }
		[[nodiscard]] const auto& getRequiredInvention() const { return requiredInvention; }
		[[nodiscard]] const auto& getInputs() const { return inputs; }
		[[nodiscard]] const auto& getOutputs() const { return outputs; }
		[[nodiscard]] const auto& getName() const { return factoryTypeName; }

		void assignTechsAndCounts(const std::map<std::string, std::string>& factoryTechMap, 
			const std::map<std::string, std::string>& factoryInventionMap, 
			const std::map<std::string, int>& startingCounts);

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