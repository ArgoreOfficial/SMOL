#pragma once

#include <string>
#include <json.hpp>

#include <sMemory.h>

#include "data/cBucketData.h"
#include "data/cExtraData.h"
#include "data/cKeyData.h"
#include "data/cEntryData.h"

namespace smol
{

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
		void loadData();

		bool m_loaded = false;

		nlohmann::json m_jdata{};

		cBucketData m_bucketData;
		cExtraData  m_extraData;
		cKeyData    m_keyData;
		cEntryData  m_entryData;


	};
}