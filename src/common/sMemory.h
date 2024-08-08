#pragma once

#include <string>
#include <stdint.h>
#include <fstream>
#include <stdio.h>

namespace smol
{
	struct sMemory 
	{
		void clear();
		void dump( const std::string& _path );
		int32_t getInt32( int _index );
		
		char* data = nullptr;
		size_t size = 0;
	};
}