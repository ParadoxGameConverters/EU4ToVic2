#include "BucketList.h"
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"

BucketList::BucketList()
{
	// Initialise with the random seed from the EU4 save so that the shuffle
	// is deterministic for particular saves, but varies between saves.
	shuffler.seed(theConfiguration.getEU4RandomSeed());

	registerKeyword(std::regex("bucket"), [this](const std::string& key, std::istream& theStream) 
		{
			Bucket newBucket(theStream);
			buckets.push_back(newBucket);
		}
	);
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseFile("configurables/rgo_randomization.txt");
}

void BucketList::putInBucket(V2Province* prov)
{
	std::string provClimate = prov->getClimate();
	if (provClimate.empty())
	{
		return;
	}
	std::string provTerrain = prov->getTerrain();
	if (provTerrain.empty() || provTerrain == "ocean")
	{
		return;
	}

	for (auto& bucket : buckets)
	{
		if (!bucket.match(provClimate, provTerrain))
		{
			continue;
		}
		bucket.addProvince(prov);
		break;
	}
}

void BucketList::shuffle()
{
	for (auto& bucket : buckets)
	{
		bucket.shuffle(shuffler);
	}
}

