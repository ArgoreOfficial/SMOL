#include "sMemory.h"

#include "Base64.h" // https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594

void smol::sMemory::free()
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
	printf( "Dumped %s (%i bytes)\n", _path.c_str(), size );
}

smol::sMemory smol::sMemory::decodeBase64( const std::string& _dataString )
{	
	std::string data;
	macaron::Base64::Decode( _dataString, data );

	smol::sMemory mem;
	mem.data = new char[ data.size() ];
	mem.size = data.size();
	memcpy( mem.data, data.data(), mem.size );

    return mem;
}

std::string smol::sMemory::encodeBase64()
{
	std::string dataString( data, size );
	return macaron::Base64::Encode( dataString );
}
