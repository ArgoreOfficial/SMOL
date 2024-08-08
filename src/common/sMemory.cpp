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