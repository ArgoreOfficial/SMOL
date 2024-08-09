#pragma once

#include <string>
#include <stdint.h>

#include <sMemory.h>
#include <json.hpp>

#include <locale>
#include <codecvt>
#include <string>

namespace smol
{
	enum eObjectType
	{
		Null = -1,

		AsciiString = 0,
		UnicodeString = 1,
		UInt16 = 2,
		UInt32 = 3,
		Int32 = 4,
		Hash128 = 5,
		Type = 6,
		JsonObject = 7
	};

	struct sJsonObject
	{
		std::string assemblyName;
		std::string className;
		std::wstring jsonText;
		nlohmann::json jsonObject;

		sMemory toMemory()
		{
			// byte : assemblyNameLength
			// string : assemblyName
			// 
			// byte : classNameLength
			// string : className
			// 
			// int32 : jsonLength
			// wstring : jsonText

			/*
			size_t totalSize = 1 + assemblyName.size() + 1 + className.size() + sizeof( int32_t ) + ( jsonText.size() * sizeof( wchar_t ) );
			sMemory mem;
			mem.data = new char[totalSize];
			mem.size = totalSize;

			size_t index = 0;
			mem.set( index, (char)assemblyName.size() );
			index++;
			
			mem.set( index, assemblyName.c_str(), assemblyName.size() );
			index += assemblyName.size();
			
			mem.set( index, (char)className.size() );
			index++;

			mem.set( index, className.c_str(), className.size() );
			index += className.size();

			int32_t jsonLength = jsonText.size() * sizeof( wchar_t );
			mem.set( index, jsonLength );
			index += sizeof( int32_t );

			mem.set( index, jsonText.c_str(), jsonLength );
			*/

			sMemory mem;
			mem.push( (char)(assemblyName.size() ) );
			mem.push( *assemblyName.c_str(), assemblyName.size() );
			mem.push( (char)className.size() );
			mem.push( *className.c_str(), className.size() );

			int32_t jsonLength = jsonText.size() * sizeof( wchar_t );
			mem.push( jsonLength );
			mem.push( *jsonText.c_str(), jsonLength );

			mem.dump( "../dumps/test.bin" );
			
			return mem;
		}
	};

	struct sObject
	{
		~sObject()
		{
			if ( type == eObjectType::AsciiString && val.asciiString )
				delete val.asciiString;
			else if ( type == eObjectType::UnicodeString && val.unicodeString )
				delete val.unicodeString;
		};

		static sObject* fromMemory( sMemory& _memory, int _index )
		{
			sObject* object = new sObject();
			object->type = static_cast<eObjectType>( _memory.data[ _index ] );
			_index++;

			switch ( object->type )
			{
			case eObjectType::UnicodeString: object->val.unicodeString = new std::string( _memory.data + _index + 4, _memory.at<int>( _index ) ); break; /// remove? unicode and ascii are identical
			case eObjectType::AsciiString:   object->val.asciiString   = new std::string( _memory.data + _index + 4, _memory.at<int>( _index ) ); break;

			case eObjectType::UInt16: object->val.uint16 = _memory.at<uint16_t>( _index ); break;
			case eObjectType::UInt32: object->val.uint32 = _memory.at<uint32_t>( _index ); break;
			case eObjectType::Int32:  object->val.int32  = _memory.at<int32_t>( _index ); break;

			case eObjectType::Hash128: break; //return Hash128.Parse( Encoding.ASCII.GetString( keyData, dataIndex + 1, keyData[ dataIndex ] ) );
			case eObjectType::JsonObject:
			{
				object->val.jsonObject = new sJsonObject();

				int assemblyNameLength = _memory.data[ _index ];
				_index++;

				std::string assemblyName = std::string( _memory.data + _index, assemblyNameLength );
				_index += assemblyNameLength;
				
				int classNameLength = _memory.data[ _index ];
				_index++;

				std::string className = std::string( _memory.data + _index, classNameLength );
				_index += classNameLength;
				
				int jsonLength = _memory.at<int32_t>( _index );
				_index += 4;
				
				std::wstring jsonText( reinterpret_cast<wchar_t*>( _memory.data + _index ), jsonLength / sizeof( wchar_t ) );

				object->val.jsonObject->assemblyName = assemblyName;
				object->val.jsonObject->className = className;
				object->val.jsonObject->jsonText = jsonText;

			} break;

			default: printf( "Error, unsuported key\n" ); break;

			}

			return object;
		}

		eObjectType type = eObjectType::Null;

		union
		{
			std::string* asciiString;
			std::string* unicodeString;

			uint16_t uint16;
			uint32_t uint32;
			int32_t int32;

			char hash128[ 16 ] = { 0 };

			bool unused_type;
			sJsonObject* jsonObject;
		} val{};
	};
}