#include "Bucket.h"
#include "ParserHelpers.h"
#include "Log.h"

Bucket::Bucket(std::istream& theStream)
{
	registerKeyword(std::regex("name"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword(std::regex("climate"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString climateStr(theStream);
			if (climateStr.getString() == "any") 
			{
				wildClimate = true;
			}
			else 
			{
				climates.push_back(climateStr.getString());
			}
		});
	registerKeyword(std::regex("terrain"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString terrainStr(theStream);
			if (terrainStr.getString() == "any")
			{
				wildTerrain = true;
			}
			else
			{
				terrains.push_back(terrainStr.getString());
			}
		});
	registerKeyword(std::regex("fraction"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleDouble fractionDbl(theStream);
			fraction = fractionDbl.getDouble();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}

bool Bucket::match(const std::string& provClimate, const std::string& provTerrain)
{
	bool climateMatch = wildClimate;
	if (!climateMatch)
	{
		for (const auto& climate : climates)
		{
			if (provClimate == climate)
			{
				climateMatch = true;
				break;
			}
		}
	}

	if (!climateMatch)
	{
		return false;
	}
	bool terrainMatch = wildTerrain;
	if (!terrainMatch)
	{
		for (const auto& terrain : terrains)
		{
			if (provTerrain == terrain)
			{
				terrainMatch = true;
				break;
			}
		}
	}

	return terrainMatch;
}

void Bucket::shuffle(std::default_random_engine& shuffler)
{
	std::shuffle(provinces.begin(), provinces.end(), shuffler);
	int numToShuffle = (int)floor(0.5 + fraction * provinces.size());
	if (numToShuffle < 2)
	{
		LOG(LogLevel::Debug) << "Skipping empty bucket " << name;
		return;
	}
	std::vector<std::string> rgos;
	for (int i = 0; i < numToShuffle; ++i)
	{
		rgos.push_back(provinces[i]->getRgoType());
	}
	std::shuffle(rgos.begin(), rgos.end(), shuffler);
	for (int i = 0; i < numToShuffle; ++i)
	{
		provinces[i]->setRgoType(rgos[i]);
	}
	LOG(LogLevel::Debug) << "Shuffled " << numToShuffle << " provinces in bucket " << name;
}
