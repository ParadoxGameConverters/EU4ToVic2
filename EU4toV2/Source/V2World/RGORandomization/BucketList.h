// Helper classes for RGO shuffle.

#ifndef BUCKET_LIST_H
#define BUCKET_LIST_H

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "../V2Province.h"
#include "Bucket.h"
#include "newParser.h"


class BucketList : commonItems::parser
{
public:
	BucketList();
	void putInBucket(V2Province* prov);
	bool empty() const { return buckets.empty(); }
	void shuffle();

private:
	std::vector<Bucket> buckets;
	std::default_random_engine shuffler;
};

#endif // BUCKET_LIST_H
