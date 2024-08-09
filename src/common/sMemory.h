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

		template<typename T>
		void set( size_t _index, const T& _data );

		template<typename T>
		void set( size_t _index, const T& _data, size_t _size );


		template<typename T> void push( const T& _data );
		template<typename T> void push( const T& _data, size_t _size );

		char* data = nullptr;
		size_t size = 0;
	};

	template<typename T>
	inline T sMemory::at( size_t _index )
	{
		if ( _index < 0 || _index >= size )
		{
			printf( "index out of range\n" );
			return T();
		}

		return *reinterpret_cast<T*>( &data[ _index ] );
	}

	template<typename T>
	inline void sMemory::set( size_t _index, const T& _data ) { set( _index, _data, sizeof( T ) ); }

	template<typename T>
	inline void sMemory::set( size_t _index, const T& _data, size_t _size )
	{
		if ( _index < 0 || _index >= size || _index + _size >= size )
		{
			printf( "index out of range\n" );
			return;
		}

		memcpy( data + _index, &_data, _size );
	}

	template<typename T>
	inline void sMemory::push( const T& _data )
	{
		push( _data, sizeof( T ) );
	}

	template<typename T>
	inline void sMemory::push( const T& _data, size_t _size )
	{
		if ( _size <= 0 )
			return;

		// create new buffer
		char* newData = new char[ size + _size ];
		if ( size > 0 )
		{
			// copy data over to new buffer
			memcpy( newData, data, size );
			// delete old buffer
			delete data;
		}
		data = newData;

		// push new data
		memcpy( data + size, &_data, _size );
		size = size + _size;
	}
}