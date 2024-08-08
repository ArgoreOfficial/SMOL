#include "sMemory.h"

void smol::sMemory::clear()
{
	delete data;
	data = nullptr;
	size = 0;
}

void smol::sMemory::dump( const std::string& _path )
{
	std::ofstream file( _path );
	file.write( data, size );
	file.close();
}

int32_t smol::sMemory::getInt32( int _index )
{
	if ( _index < 0 || _index >( int )size )
	{
		printf( "getInt32 index out of range\n" );
		return 0;
	}

	return *reinterpret_cast<int*>( &data[ _index ] );
}
