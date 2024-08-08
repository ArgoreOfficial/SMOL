#include "cCatalog.h"

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include "Base64.h" // https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594

void smol::cCatalog::loadFromFile( const std::string& _path )
{
	m_jdata = loadCatalogJson( _path );
	if ( m_jdata.empty() )
		return;
	
	printf( "Loaded Catalog\n" );

	createLocator();
	printf( "Created Locator\n" );

	smol::sMemory keyData = getCatalogMemory( "m_KeyDataString" );
	smol::sMemory entryData = getCatalogMemory( "m_EntryDataString" );

	keyData.dump( "../dumps/m_KeyDataString.bin" );
	printf( "Dumped m_KeyDataString\n" );
	entryData.dump( "../dumps/m_EntryDataString.bin" );
	printf( "Dumped m_EntryDataString\n" );

	m_loaded = true;
}

nlohmann::json smol::cCatalog::loadCatalogJson( const std::string& _path )
{
	std::fstream fstream( _path );
	if ( !fstream.is_open() )
	{
		printf( "Couldn't find file '%s'\n", _path.c_str() );
		return {};
	}

	std::stringstream buffer;
	buffer << fstream.rdbuf();

	nlohmann::json catalog = nlohmann::json::parse( buffer.str() );

	return catalog;
}

smol::sMemory smol::cCatalog::getCatalogMemory( std::string _key )
{
	std::string dataString = m_jdata.at( _key );
	std::string data;

	macaron::Base64::Decode( dataString, data );
	smol::sMemory mem;
	mem.data = new char[ data.size() ];
	mem.size = data.size();
	memcpy( mem.data, data.data(), mem.size );

	return mem;
}

void smol::cCatalog::createLocator()
{
	m_bucketData = getCatalogMemory( "m_BucketDataString" );
	int numBuckets = m_bucketData.getInt32( 0 );
	m_buckets.resize( numBuckets );

	int bi = 4;
	for ( int i = 0; i < numBuckets; i++ )
	{
		int index = m_bucketData.getInt32( bi );
		bi += 4;
		int numEntries = m_bucketData.getInt32( bi );
		bi += 4;
		int* entryArray = new int[ numEntries ];
		for ( int c = 0; c < numEntries; c++ )
		{
			entryArray[ c ] = m_bucketData.getInt32( bi );
			bi += 4;
		}

		m_buckets[ i ].pEntries = entryArray;
		m_buckets[ i ].numEntries = numEntries;
		m_buckets[ i ].dataOffset = index;
	}

	printf( "Loaded %i buckets\n", numBuckets );


	m_extraData = getCatalogMemory( "m_ExtraDataString" );
	m_keyData = getCatalogMemory( "m_KeyDataString" );
	int numKeys = m_keyData.getInt32( 0 );
	// var keys = new object[ numKeys ];
	//for ( int i = 0; i < buckets.Length; i++ )
	//	keys[ i ] = SerializationUtilities.ReadObjectFromByteArray( keyData, buckets[ i ].dataOffset );

	printf( "Loaded %i keys\n", numKeys );

	//var locator = new ResourceLocationMap( m_LocatorId, buckets.Length );

	m_entryData = getCatalogMemory( "m_EntryDataString" );
	int numEntries = m_entryData.getInt32( 0 );
	//var locations = new IResourceLocation[ count ];
	for ( int i = 0; i < numEntries; i++ )
	{
		int index = m_cBytesPerInt32 + i * ( m_cBytesPerInt32 * m_cEntryDataItemPerEntry );
		int internalId = m_entryData.getInt32( index );
		index += m_cBytesPerInt32;
		int providerIndex = m_entryData.getInt32( index );
		index += m_cBytesPerInt32;
		int dependencyKeyIndex = m_entryData.getInt32( index );
		index += m_cBytesPerInt32;
		int depHash = m_entryData.getInt32( index );
		index += m_cBytesPerInt32;
		int dataIndex = m_entryData.getInt32( index );
		index += m_cBytesPerInt32;
		int primaryKey = m_entryData.getInt32( index );
		index += m_cBytesPerInt32;
		int resourceType = m_entryData.getInt32( index );

		//object data = dataIndex < 0 ? null : SerializationUtilities.ReadObjectFromByteArray( extraData, dataIndex );
		//locations[ i ] = new CompactLocation( locator, Addressables.ResolveInternalId( ExpandInternalId( m_InternalIdPrefixes, m_InternalIds[ internalId ] ) ),
		//									  m_ProviderIds[ providerIndex ], dependencyKeyIndex < 0 ? null : keys[ dependencyKeyIndex ], data, depHash, keys[ primaryKey ].ToString(), m_resourceTypes[ resourceType ].Value );
	}

	printf( "Loaded %i Entries\n", numEntries );

	for ( int i = 0; i < m_buckets.size(); i++ )
	{
		sBucket& bucket = m_buckets[ i ];
		//var key = keys[ i ];
		//var locs = new IResourceLocation[ bucket.entries.Length ];
		//for ( int b = 0; b < bucket.entries.Length; b++ )
		//	locs[ b ] = locations[ bucket.entries[ b ] ];
		//locator.Add( key, locs );
	}

}
