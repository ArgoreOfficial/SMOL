#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include <sMemory.h>

#include "cCatalog.h"

void printKeyData( smol::sMemory& _keyData )
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

void printEntryData( smol::sMemory& _entryData )
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


	int count = _entryData.getInt32( 0 );
	
	const int bytesPerInt32 = sizeof( int );
	const int entryDataItemPerEntry = 7;

	//var locations = new IResourceLocation[ count ];
	for ( int i = 0; i < count; i++ )
	{
		int index = bytesPerInt32 + i * ( bytesPerInt32 * entryDataItemPerEntry );
		int internalId = _entryData.getInt32( index );
		index += bytesPerInt32;
		int providerIndex = _entryData.getInt32( index );
		index += bytesPerInt32;
		int dependencyKeyIndex = _entryData.getInt32( index );
		index += bytesPerInt32;
		int depHash = _entryData.getInt32( index );
		index += bytesPerInt32;
		int dataIndex = _entryData.getInt32( index );
		index += bytesPerInt32;
		int primaryKey = _entryData.getInt32( index );
		index += bytesPerInt32;
		int resourceType = _entryData.getInt32( index );

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
	
	smol::cCatalog catalog;
	catalog.loadFromFile( "../dat/catalog.json" );
	if ( !catalog.loaded() )
	{
		printf( "Failed to load catalog, aborting\n" );
		return 0;
	}

	return 0;
}