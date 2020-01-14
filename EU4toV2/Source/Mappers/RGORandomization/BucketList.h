#ifndef BUCKET_LIST_H
#define BUCKET_LIST_H

#include <random>
#include <vector>
#include "../../V2World/V2Province.h"
#include "Bucket.h"
#include "newParser.h"

namespace mappers
{
	// Helper classes for RGO shuffle.
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
}

#endif // BUCKET_LIST_H
