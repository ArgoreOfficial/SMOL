#pragma once

#include <string>
#include <json.hpp>

#include <sMemory.h>

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

	struct sEntry
	{
		int internalId;
		int providerIndex;
		int dependencyKeyIndex;
		int depHash;
		int dataIndex;
		int primaryKey;
		int resourceType;
	};

	struct sBucket
	{
		~sBucket()
		{
			if ( pEntries )
			{
				delete[] pEntries;
				pEntries = nullptr;
			}
		}

		int dataOffset = 0;
		int* pEntries = nullptr;
		size_t numEntries = 0;
	};


	struct sKeyObject
	{
		~sKeyObject() 
		{
			if ( type == eObjectType::AsciiString && val.asciiString ) 
				delete val.asciiString;
			else if ( type == eObjectType::UnicodeString && val.unicodeString )
				delete val.unicodeString;

		};

		eObjectType type;

		union
		{
			std::string* asciiString;
			std::string* unicodeString;

			uint16_t uint16;
			uint32_t uint32;
			int32_t int32;
			
			char hash128[ 16 ] = { 0 };

			bool unused_type;
			bool unused_jsonObject;
		} val;
	};

	class cCatalog
	{
	public:

		 cCatalog() {}
		~cCatalog() {}

		void loadFromFile( const std::string& _path );
		bool loaded() { return m_loaded; }

	private:

		nlohmann::json loadCatalogJson( const std::string& _path );
		sMemory getCatalogMemory( std::string _key );
		void resolveKey( int _index, sKeyObject& _key );
		void createLocator();

		bool m_loaded = false;

		nlohmann::json m_jdata{};

		sMemory m_bucketData;
		sMemory m_extraData;
		sMemory m_keyData;
		sMemory m_entryData;

		std::vector<sBucket> m_buckets;
		std::vector<sKeyObject> m_keys;
		std::vector<sEntry> m_entries;

		const int m_cBytesPerInt32 = 4;
		const int m_cEntryDataItemPerEntry = 7;


	};
}