#pragma once

#include <string>
#include <json.hpp>

#include <sMemory.h>

namespace smol
{
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

	class cCatalog
	{
	public:

		 cCatalog() {}
		~cCatalog() {}

		void loadFromFile( const std::string& _path );
		bool loaded() { return m_loaded; }

	private:

		nlohmann::json loadCatalogJson( const std::string& _path );
		smol::sMemory getCatalogMemory( std::string _key );

		void createLocator();

		bool m_loaded = false;

		nlohmann::json m_jdata{};

		smol::sMemory m_bucketData;
		smol::sMemory m_extraData;
		smol::sMemory m_keyData;
		smol::sMemory m_entryData;

		std::vector<sBucket> m_buckets;

		const int m_cBytesPerInt32 = 4;
		const int m_cEntryDataItemPerEntry = 7;


	};
}