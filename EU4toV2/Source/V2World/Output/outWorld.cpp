#include "outWorld.h"
#include "../V2World.h"

std::ostream& V2::operator<<(std::ostream& output, const std::optional<std::pair<int, std::vector<std::shared_ptr<V2::Pop>>>>& pops)
{
	if (!pops) return output;
	if (!pops->first) return output;
	
	output << pops->first << " = {\n";
	for (const auto& pop : pops->second)
	{
		output << *pop;
	}
	output << "}\n";

	return output;
}

