#pragma once

#include <string>
#include <stdint.h>
#include <fstream>
#include <stdio.h>

namespace smol
{
	struct sMemory 
	{
		void free();
		void dump( const std::string& _path );
		
		static sMemory decodeBase64( const std::string& _dataString );
		std::string encodeBase64();

		template<typename T>
		T at( size_t _index );

		char* data = nullptr;
		size_t size = 0;
	};

	template<typename T>
	inline T sMemory::at( size_t _index )
	{
		if ( _index < 0 || _index => size )
		{
			printf( "index out of range\n" );
			return T();
		}

		return *reinterpret_cast<T*>( &data[ _index ] );
	}
}