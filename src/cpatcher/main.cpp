#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include "json.hpp"
#include "Base64.h" // https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594

struct sMemory 
{
	char* data = nullptr;
	size_t size = 0;

	void clear()
	{
		delete data;
		data = nullptr;
		size = 0;
	}

	void dump( const std::string& _path )
	{
		std::ofstream file( _path );
		file.write( data, size );
		file.close();
	}
};

nlohmann::json loadCatalog( const std::string& _path )
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

sMemory getCatalogMemory( nlohmann::json& _catalog, std::string _key )
{
	std::string dataString = _catalog.at( _key );
	std::string data;

	macaron::Base64::Decode( dataString, data );
	sMemory mem;
	mem.data = new char[ data.size() ];
	mem.size = data.size();
	memcpy( mem.data, data.data(), mem.size );

	return mem;
}

void printKeyData( sMemory& _keyData )
{
	// THIS IS REALLY DANGEROUS
	int numStrings = 0;
	for ( size_t i = 5; i < _keyData.size; i++ )
	{
		int strSize = *reinterpret_cast<int*>( &_keyData.data[ i ] );
		i += sizeof( int );

		printf( "[%04i] [%05i] %s\n", numStrings, (int)i, &_keyData.data[ i ] );

		i += strSize;
		numStrings++;
	}
}

int readInt32FromMemory( sMemory& _mem, int _index )
{
	return *reinterpret_cast<int*>( &_mem.data[ _index ] );
}

void printEntryData( sMemory& _entryData )
{
	//for ( size_t i = 0; i < _entryData.size; i += sizeof(int) )
	//{
	//	int val = *reinterpret_cast<int*>( &_entryData.data[ i ] );
	//	
	//	if ( val != 0xFFFFFFFF )
	//		printf( "%i ", val );
	//	else
	//		printf( "\n" );
	//}


	int count = readInt32FromMemory( _entryData, 0 );
	
	const int bytesPerInt32 = sizeof( int );
	const int entryDataItemPerEntry = 7;

	//var locations = new IResourceLocation[ count ];
	for ( int i = 0; i < count; i++ )
	{
		int index = bytesPerInt32 + i * ( bytesPerInt32 * entryDataItemPerEntry );
		int internalId = readInt32FromMemory( _entryData, index );
		index += bytesPerInt32;
		int providerIndex = readInt32FromMemory( _entryData, index );
		index += bytesPerInt32;
		int dependencyKeyIndex = readInt32FromMemory( _entryData, index );
		index += bytesPerInt32;
		int depHash = readInt32FromMemory( _entryData, index );
		index += bytesPerInt32;
		int dataIndex = readInt32FromMemory( _entryData, index );
		index += bytesPerInt32;
		int primaryKey = readInt32FromMemory( _entryData, index );
		index += bytesPerInt32;
		int resourceType = readInt32FromMemory( _entryData, index );

		printf( "internalId:         %i\n", internalId );
		printf( "providerIndex:      %i\n", providerIndex );
		printf( "dependencyKeyIndex: %i\n", dependencyKeyIndex );
		printf( "depHash:            %i\n", depHash );
		printf( "dataIndex:          %i\n", dataIndex );
		printf( "primaryKey:         %i\n", primaryKey );
		printf( "resourceType:       %i\n", resourceType );

		//object data = dataIndex < 0 ? null : SerializationUtilities.ReadObjectFromByteArray( extraData, dataIndex );
		//locations[ i ] = new CompactLocation( locator, Addressables.ResolveInternalId( ExpandInternalId( m_InternalIdPrefixes, m_InternalIds[ internalId ] ) ),
		//									  m_ProviderIds[ providerIndex ], dependencyKeyIndex < 0 ? null : keys[ dependencyKeyIndex ], data, depHash, keys[ primaryKey ].ToString(), m_resourceTypes[ resourceType ].Value );
	}


}

int main()
{
	printf("Loading Catalog\n");
	nlohmann::json catalog = loadCatalog( "../dat/catalog.json" );
	if ( catalog.empty() )
	{
		printf( "Failed to open catalog, aborting\n" );
		return 0;
	}

	printf( "Parsing Data\n" );
	sMemory keyData   = getCatalogMemory( catalog, "m_KeyDataString" );
	sMemory entryData = getCatalogMemory( catalog, "m_EntryDataString" );
	
	keyData.dump( "../dumps/m_KeyDataString.bin" );
	entryData.dump( "../dumps/m_EntryDataString.bin" );

	printEntryData( entryData );

	return 0;
}