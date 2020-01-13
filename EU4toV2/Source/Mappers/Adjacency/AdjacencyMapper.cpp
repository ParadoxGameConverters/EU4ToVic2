#include "AdjacencyMapper.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>

mappers::AdjacencyMapper::AdjacencyMapper()
{
	LOG(LogLevel::Info) << "Importing province adjacencies.";
	std::string filename = getAdjacencyFilename();

	std::ifstream adjacenciesFile(filename, std::ios_base::binary);
	if (!adjacenciesFile.is_open())
	{
		std::runtime_error exception("Could not open " + filename + ", aborting!");
		throw exception;
	}

	inputAdjacencies(adjacenciesFile);
	adjacenciesFile.close();

	if (theConfiguration.getDebug())
	{
		outputAdjacenciesMapData();
	}
}

std::string mappers::AdjacencyMapper::getAdjacencyFilename()
{
	std::string filename = theConfiguration.getVic2DocumentsPath() + "/map/cache/adjacencies.bin";
	if (!Utils::DoesFileExist(filename))
	{
		LOG(LogLevel::Warning) << "Could not find " << filename << " - looking in install folder.";
		filename = theConfiguration.getVic2Path() + "/map/cache/adjacencies.bin";
		if (!Utils::DoesFileExist(filename))
		{
			std::runtime_error exception("Could not find " + filename + ". Try running Vic2 and converting again.");
			throw exception;
		}
		else
		{
			LOG(LogLevel::Warning) << "No need to worry, it was there all along.";
		}
	}
	return filename;
}

void mappers::AdjacencyMapper::inputAdjacencies(std::istream& adjacenciesFile)
{
	int current_province = 0;
	while (!adjacenciesFile.eof())
	{
		char bitBucket[4];
		adjacenciesFile.read(bitBucket, sizeof(bitBucket));

		std::vector<int> adjacencies = readAnAdjacenciesSet(adjacenciesFile, char4toint(bitBucket));
		adjacencyMap.insert(std::make_pair(current_province, adjacencies));
		current_province++;
	}
}

uint32_t char4toint(char *buffer)
{
	return (uint32_t)buffer[0] << 24 | (uint32_t)buffer[1] << 16 | (uint32_t)buffer[2] << 8 | (uint32_t)buffer[3];
}

std::istream& operator >> (std::istream& stream, mappers::HODAdjacency& adjacency)
{
	char bitBucket[4];
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.type = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.to = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.via = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown1 = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown2 = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.pathX = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.pathY = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown3 = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown4 = char4toint(bitBucket);

	return stream;
};

std::istream& operator >> (std::istream& stream, mappers::AHDAdjacency& adjacency)
{
	char bitBucket[4];
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.type = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.to = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.via = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown1 = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown2 = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.pathX = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.pathY = char4toint(bitBucket);

	return stream;
}

std::istream& operator >> (std::istream& stream, mappers::VanillaAdjacency& adjacency)
{
	char bitBucket[4];
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.type = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.to = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.via = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown1 = char4toint(bitBucket);
	stream.read(bitBucket, sizeof(bitBucket));
	adjacency.unknown2 = char4toint(bitBucket);

	return stream;
}

std::vector<int> mappers::AdjacencyMapper::readAnAdjacenciesSet(std::istream& adjacenciesFile, unsigned int numAdjacencies)
{
	std::vector<int> adjacencies;
	for (unsigned int i = 0; i < numAdjacencies; i++)
	{
		if (theConfiguration.getVic2Gametype() == "vanilla")
		{
			VanillaAdjacency readAdjacency;
			adjacenciesFile >> readAdjacency;
			adjacencies.push_back(readAdjacency.to);
		}
		else if (theConfiguration.getVic2Gametype() == "AHD")
		{
			AHDAdjacency readAdjacency;
			adjacenciesFile >> readAdjacency;
			adjacencies.push_back(readAdjacency.to);
		}
		if ((theConfiguration.getVic2Gametype() == "HOD") || (theConfiguration.getVic2Gametype() == "HoD-NNM"))
		{
			HODAdjacency readAdjacency;
			adjacenciesFile >> readAdjacency;
			adjacencies.push_back(readAdjacency.to);
		}
	}

	return adjacencies;
}

void mappers::AdjacencyMapper::outputAdjacenciesMapData()
{
	std::ofstream adjacenciesData("adjacenciesData.csv");

	adjacenciesData << "From,To\n";
	for (auto adjacencyMapping: adjacencyMap)
	{
		for (auto adjacency: adjacencyMapping.second)
		{
			adjacenciesData << adjacencyMapping.first << "," << adjacency << "\n";
		}
	}

	adjacenciesData.close();
}

std::optional<std::vector<int>> mappers::AdjacencyMapper::getVic2Adjacencies(int Vic2Province) const
{
	if(adjacencyMap.find(Vic2Province) != adjacencyMap.end())
	{
		return adjacencyMap.at(Vic2Province);
	}
	else
	{
		return std::nullopt;
	}
}