#include "cCatalog.h"

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include "Base64.h" // https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594

///////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////

void smol::cCatalog::resolveKey( int _index, sKeyObject& _key )
{
	_key.type = static_cast<eObjectType>( m_keyData.data[ _index ] );

	switch ( _key.type )
	{
	case eObjectType::UnicodeString: /// remove? unicode and ascii are identical
	{
		//var dataLength = BitConverter.ToInt32( keyData, dataIndex );
		//return Encoding.Unicode.GetString( keyData, dataIndex + 4, dataLength );
		int stringLength = m_keyData.at<int>( _index );
		_key.val.unicodeString = new std::string( m_keyData.data + _index + 4, stringLength );
	} break;
	case eObjectType::AsciiString:
	{
		//var dataLength = BitConverter.ToInt32( keyData, dataIndex );
		//return Encoding.ASCII.GetString( keyData, dataIndex + 4, dataLength );
		int stringLength = m_keyData.at<int>( _index );
		_key.val.asciiString = new std::string( m_keyData.data + _index + 4, stringLength );
	} break;

	case eObjectType::UInt16: _key.val.uint16 = m_keyData.at<uint16_t>( _index ); break;
	case eObjectType::UInt32: _key.val.uint32 = m_keyData.at<uint32_t>( _index ); break;
	case eObjectType::Int32:  _key.val.int32  = m_keyData.at<int32_t> ( _index ); break;

	case eObjectType::Hash128: break; //return Hash128.Parse( Encoding.ASCII.GetString( keyData, dataIndex + 1, keyData[ dataIndex ] ) );
	
	case eObjectType::Type: 
	case eObjectType::JsonObject: printf( "Error, unsuported key\n" ); break;

	}
}

///////////////////////////////////////////////////////////////////////////////////////

void smol::cCatalog::createLocator()
{
	m_bucketData = getCatalogMemory( "m_BucketDataString" );
	int numBuckets = m_bucketData.at<int32_t>( 0 );
	m_buckets.resize( numBuckets );

	int bi = 4;
	for ( int i = 0; i < numBuckets; i++ )
	{
		int index = m_bucketData.at<int32_t>( bi );
		bi += 4;
		int numEntries = m_bucketData.at<int32_t>( bi );
		bi += 4;
		int* entryArray = new int[ numEntries ];
		for ( int c = 0; c < numEntries; c++ )
		{
			entryArray[ c ] = m_bucketData.at<int32_t>( bi );
			bi += 4;
		}

		m_buckets[ i ].pEntries = entryArray;
		m_buckets[ i ].numEntries = numEntries;
		m_buckets[ i ].dataOffset = index;
	}

	printf( "Loaded %i buckets\n", numBuckets );


	m_extraData = getCatalogMemory( "m_ExtraDataString" );
	m_keyData   = getCatalogMemory( "m_KeyDataString" );
	int numKeys = m_keyData.at<int32_t>( 0 );
	m_keys.resize( numKeys );
	for ( int i = 0; i < m_buckets.size(); i++ )
		resolveKey( m_buckets[ i ].dataOffset, m_keys[ i ] );
	
	printf( "Loaded %i keys\n", numKeys );

	m_entryData = getCatalogMemory( "m_EntryDataString" );
	int numEntries = m_entryData.at<int32_t>( 0 );
	m_entries.resize( numEntries );

	for ( int i = 0; i < numEntries; i++ )
	{
		int index = m_cBytesPerInt32 + i * ( m_cBytesPerInt32 * m_cEntryDataItemPerEntry );
		m_entries[ i ].internalId = m_entryData.at<int32_t>( index );
		index += m_cBytesPerInt32;
		m_entries[ i ].providerIndex = m_entryData.at<int32_t>( index );
		index += m_cBytesPerInt32;
		m_entries[ i ].dependencyKeyIndex = m_entryData.at<int32_t>( index );
		index += m_cBytesPerInt32;
		m_entries[ i ].depHash = m_entryData.at<int32_t>( index );
		index += m_cBytesPerInt32;
		m_entries[ i ].dataIndex = m_entryData.at<int32_t>( index );
		index += m_cBytesPerInt32;
		m_entries[ i ].primaryKey = m_entryData.at<int32_t>( index );
		index += m_cBytesPerInt32;
		m_entries[ i ].resourceType = m_entryData.at<int32_t>( index );
	}

	printf( "Loaded %i Entries\n", numEntries );

}
