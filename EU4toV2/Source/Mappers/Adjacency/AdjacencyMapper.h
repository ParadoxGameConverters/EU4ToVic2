#ifndef ADJACENCY_MAPPER_H
#define ADJACENCY_MAPPER_H

#include <map>
#include <optional>
#include <vector>
#include <string>

namespace mappers
{
	typedef struct
	{
		uint32_t type; // the type of adjacency 0 = normal, 1 = ford, 2 = river crossing
		uint32_t to; // the province this one is adjacent to (expect one pointing back to this province)
		uint32_t via; // the straight (if any) this crosses
		uint32_t unknown1; // still unknown
		uint32_t unknown2; // still unknown
		uint32_t pathX; // the midpoint on the path drawn between provinces
		uint32_t pathY; // the midpoint on the path drawn between provinces
		uint32_t unknown3; // still unknown
		uint32_t unknown4; // still unknown
	} Adjacency; // an entry in the HOD adjacencies.bin format

	class AdjacencyMapper
	{
	public:
		AdjacencyMapper();
		std::optional<std::vector<int>> getVic2Adjacencies(int Vic2Province) const;

	private:
		std::string getAdjacencyFilename();
		void inputAdjacencies(std::istream& adjacenciesFile);
		std::vector<int> readAnAdjacenciesSet(std::istream& adjacenciesFile, unsigned int numAdjacencies);
		void outputAdjacenciesMapData();

		std::map<int, std::vector<int>> adjacencyMap;
	};
}

std::istream& operator >> (std::istream& stream, mappers::Adjacency& adjacency);

#endif // ADJACENCY_MAPPER_H