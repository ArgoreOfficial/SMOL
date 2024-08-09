#pragma once

#include "iCatalogData.h"

#include <vector>

namespace smol
{
	struct sBucket
	{
		~sBucket()
		{
			if ( pEntries )
			{
				delete[] pEntries;
				pEntries = nullptr;
			}
		}

		int dataOffset = 0;
		size_t numEntries = 0;
		int* pEntries = nullptr;
	};

	class cBucketData
	{
	public:
		 cBucketData() { }
		~cBucketData() { }

		void    fromMemory( sMemory& _memory );
		sMemory toMemory  ( void );

		size_t bucketCount() { return m_buckets.size(); }
		sBucket& get( size_t _index ) { return m_buckets[ _index ]; }

	private:
		
		std::vector<sBucket> m_buckets;

	};
}