#include "AdjacencyMapper.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>

mappers::AdjacencyMapper::AdjacencyMapper()
{
	LOG(LogLevel::Info) << "Importing province adjacencies.";
	const auto& filename = getAdjacencyFilename();

	std::ifstream adjacenciesFile(filename, std::ios_base::binary);
	if (!adjacenciesFile.is_open()) throw std::runtime_error("Could not open " + filename + ", aborting!");

	inputAdjacencies(adjacenciesFile);
	adjacenciesFile.close();

	if (theConfiguration.getDebug())
	{
		outputAdjacenciesMapData();
	}
}

std::string mappers::AdjacencyMapper::getAdjacencyFilename()
{
	auto filename = theConfiguration.getVic2DocumentsPath() + "/map/cache/adjacencies.bin";
	if (!Utils::DoesFileExist(filename))
	{
		LOG(LogLevel::Warning) << "Could not find " << filename << " - looking in install folder.";
		filename = theConfiguration.getVic2Path() + "/map/cache/adjacencies.bin";
		if (!Utils::DoesFileExist(filename)) throw std::runtime_error("Could not find " + filename + ". Try running Vic2 and converting again.");
		LOG(LogLevel::Warning) << "No need to worry, it was there all along.";
	}
	return filename;
}

void mappers::AdjacencyMapper::inputAdjacencies(std::istream& adjacenciesFile)
{
	auto currentProvince = 0;
	while (!adjacenciesFile.eof())
	{
		uint32_t numAdjacencies;
		adjacenciesFile.read(reinterpret_cast<char*>(&numAdjacencies), 4);

		const auto& adjacencies = readAnAdjacenciesSet(adjacenciesFile, numAdjacencies);
		adjacencyMap.insert(std::make_pair(currentProvince, adjacencies));
		currentProvince++;
	}
}

std::istream& operator >> (std::istream& stream, mappers::Adjacency& adjacency)
{
	stream.read(reinterpret_cast<char*>(&adjacency.type), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.to), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.via), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.unknown1), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.unknown2), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.pathX), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.pathY), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.unknown3), 4);
	stream.read(reinterpret_cast<char*>(&adjacency.unknown4), 4);

	return stream;
};

std::vector<int> mappers::AdjacencyMapper::readAnAdjacenciesSet(std::istream& adjacenciesFile, const unsigned int numAdjacencies)
{
	std::vector<int> adjacencies;
	for (unsigned int i = 0; i < numAdjacencies; i++)
	{
		Adjacency readAdjacency;
		adjacenciesFile >> readAdjacency;
		adjacencies.push_back(readAdjacency.to);
	}
	return adjacencies;
}

void mappers::AdjacencyMapper::outputAdjacenciesMapData()
{
	std::ofstream adjacenciesData("adjacenciesData.csv");

	adjacenciesData << "From,To\n";
	for (const auto& adjacencyMapping: adjacencyMap)
	{
		for (const auto& adjacency: adjacencyMapping.second)
		{
			adjacenciesData << adjacencyMapping.first << "," << adjacency << "\n";
		}
	}

	adjacenciesData.close();
}

std::optional<std::vector<int>> mappers::AdjacencyMapper::getVic2Adjacencies(const int vic2Province) const
{
	if(adjacencyMap.find(vic2Province) != adjacencyMap.end()) return adjacencyMap.at(vic2Province);
	return std::nullopt;
}