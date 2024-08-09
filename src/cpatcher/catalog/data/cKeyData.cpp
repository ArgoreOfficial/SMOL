#include "cKeyData.h"

#include "cBucketData.h"

void smol::cKeyData::fromMemory( sMemory& _memory, cBucketData* _bucketData )
{
	int numKeys = _memory.at<int32_t>( 0 );

	m_keys.reserve( numKeys );

	for ( int i = 0; i < _bucketData->bucketCount(); i++ )
		m_keys[ i ] = sObject::fromMemory( _memory, _bucketData->get( i ).dataOffset );
	
	printf( "Loaded %i Keys\n", numKeys );

}

smol::sMemory smol::cKeyData::toMemory( void )
{
	/// TODO: implement
	return sMemory{};
}