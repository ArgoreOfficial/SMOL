#include "cCatalog.h"

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////

void smol::cCatalog::loadFromFile( const std::string& _path )
{
	m_jdata = loadCatalogJson( _path );
	if ( m_jdata.empty() )
		return;
	
	printf( "Loaded Catalog\n" );

	loadData();
	printf( "Created Locator\n" );

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

void smol::cCatalog::loadData()
{
	// load memory blocks
	sMemory bucketMemory = sMemory::decodeBase64( m_jdata.at( "m_BucketDataString" ) );
	sMemory extraMemory  = sMemory::decodeBase64( m_jdata.at( "m_ExtraDataString"  ) );
	sMemory keyMemory    = sMemory::decodeBase64( m_jdata.at( "m_KeyDataString"    ) );
	sMemory entryMemory  = sMemory::decodeBase64( m_jdata.at( "m_EntryDataString"  ) );
	
	// parse data
	m_bucketData.fromMemory( bucketMemory );
	m_extraData .fromMemory( extraMemory );
	m_keyData   .fromMemory( keyMemory, &m_bucketData );
	m_entryData .fromMemory( entryMemory, extraMemory );

	// dump binaries
	bucketMemory.dump( "../dumps/m_BucketDataString.bin" );
	extraMemory .dump( "../dumps/m_ExtraDataString.bin" );
	keyMemory   .dump( "../dumps/m_KeyDataString.bin" );
	entryMemory .dump( "../dumps/m_EntryDataString.bin" );
	
	// free memory blocks
	bucketMemory.free();
	extraMemory .free();
	keyMemory   .free();
	entryMemory .free();
}
