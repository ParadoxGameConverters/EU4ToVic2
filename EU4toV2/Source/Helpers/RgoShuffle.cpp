#include "RgoShuffle.h"

#include "../Configuration.h"
#include "Log.h"
#include "NewParserToOldParserConverters.h"
#include "../V2World/V2Province.h"

Bucket::Bucket(const std::string& n, double f) : name(n), fraction(f) {}

BucketList::BucketList(std::istream& theStream)
{
	// Initialise with the random seed from the EU4 save so that the shuffle
	// is deterministic for particular saves, but varies between saves.
	shuffler.seed(theConfiguration.getEU4RandomSeed());
	registerKeyword(
	    std::regex("bucket"),
	    [this](const std::string& key, std::istream& theStream) {
		    auto top = commonItems::convert8859Object(key, theStream);
                    auto obj = top->safeGetObject("bucket");
		    if (!obj)
		    {
			    LOG(LogLevel::Warning)
			        << "Bucket parsing went wrong";
			    return;
		    }
		    auto climates = obj->getValue("climate");
		    if (climates.empty())
		    {
			    LOG(LogLevel::Warning)
			        << "Discarding bucket with no climates.";
			    return;
		    }
		    auto terrains = obj->getValue("terrain");
		    if (terrains.empty())
		    {
			    LOG(LogLevel::Warning)
			        << "Discarding bucket with no terrains.";
			    return;
		    }
                    std::string bucketName = obj->safeGetString("name");
		    if (bucketName.empty())
		    {
			    bucketName = terrains[0]->getLeaf() + "_" +
			                 climates[0]->getLeaf();
		    }
		    auto fraction = obj->safeGetFloat("fraction", 0.0);
		    Bucket bucket(bucketName, fraction);
		    for (auto& c : climates)
		    {
                            bucket.addClimate(c->getLeaf());
		    }
		    for (auto& t : terrains)
		    {
                            bucket.addTerrain(t->getLeaf());
		    }
                    buckets.push_back(bucket);
	    });
	parseStream(theStream);
}

bool Bucket::match(const string& provClimate,
                               const string& provTerrain)
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

void BucketList::putInBucket(V2Province* prov)
{
	string provClimate = prov->getClimate();
	if (provClimate.empty())
	{
		return;
	}
	string provTerrain = prov->getTerrain();
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

void Bucket::addClimate(const std::string& c) {
  if (c == "any") {
    wildClimate = true;
  } else {
    climates.push_back(c);
  }
}

void Bucket::addTerrain(const std::string& t) {
  if (t == "any") {
    wildTerrain = true;
  } else {
    terrains.push_back(t);
  }
}

void Bucket::addProvince(V2Province* prov) { provinces.push_back(prov); }

void Bucket::shuffle(std::default_random_engine& shuffler)
{
	std::shuffle(provinces.begin(), provinces.end(), shuffler);
	int numToShuffle = (int)floor(0.5 + fraction * provinces.size());
	if (numToShuffle < 2)
	{
		LOG(LogLevel::Info) << "Skipping empty bucket " << name;
                return;
	}
	std::vector<string> rgos;
	for (int i = 0; i < numToShuffle; ++i)
	{
		rgos.push_back(provinces[i]->getRgoType());
	}
	std::shuffle(rgos.begin(), rgos.end(), shuffler);
	for (int i = 0; i < numToShuffle; ++i)
	{
		provinces[i]->setRgoType(rgos[i]);
	}
	LOG(LogLevel::Info)
	    << "Shuffled " << numToShuffle << " provinces in bucket " << name;
}

void BucketList::shuffle()
{
	for (auto& bucket : buckets)
	{
		bucket.shuffle(shuffler);
        }
}

