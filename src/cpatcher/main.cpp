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