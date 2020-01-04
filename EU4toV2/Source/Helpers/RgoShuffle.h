// Helper classes for RGO shuffle.

#ifndef RGO_SHUFFLE_H
#define RGO_SHUFFLE_H

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "newParser.h"

class V2Province;

class Bucket
{
      public:
	Bucket(const std::string& n, double f);
	bool match(const std::string& provClimate,
	           const std::string& provTerrain);
	void shuffle(std::default_random_engine& shuffler);
	void addClimate(const std::string& c);
	void addProvince(V2Province* prov);
	void addTerrain(const std::string& t);

      private:
	std::string name;
	std::vector<std::string> climates = {};
	std::vector<std::string> terrains = {};
	double fraction = 0.0;
	bool wildClimate = false;
	bool wildTerrain = false;
	std::vector<V2Province*> provinces;
};

class BucketList : private commonItems::parser
{
      public:
	explicit BucketList(std::istream& theStream);
	void putInBucket(V2Province* prov);
	bool empty() const { return buckets.empty(); }
	void shuffle();

      private:
	std::vector<Bucket> buckets;
	std::default_random_engine shuffler;
};

#endif
