#include "cBucketData.h"

void smol::cBucketData::fromMemory( sMemory& _memory )
{
	int numBuckets = _memory.at<int32_t>( 0 );
	m_buckets.resize( numBuckets );

	/// could probably be refactored into _memory.at<sBucket>( pointer )

	int pointer = 4;
	for ( int i = 0; i < numBuckets; i++ )
	{
		int index = _memory.at<int32_t>( pointer );
		pointer += 4;
		int numEntries = _memory.at<int32_t>( pointer );
		pointer += 4;
		int* entryArray = new int[ numEntries ];
		for ( int c = 0; c < numEntries; c++ )
		{
			entryArray[ c ] = _memory.at<int32_t>( pointer );
			pointer += 4;
		}

		m_buckets[ i ].dataOffset = index;
		m_buckets[ i ].numEntries = numEntries;
		m_buckets[ i ].pEntries = entryArray;
	}

	printf( "Loaded %i Buckets\n", numBuckets );
}

smol::sMemory smol::cBucketData::toMemory()
{
	/// TODO: implement
	return sMemory{};
}
