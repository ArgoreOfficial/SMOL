#pragma once

#include "iCatalogData.h"

#include <vector>
#include <sObject.h>

namespace smol
{
	struct sEntry
	{
		int internalId = 0;
		int providerIndex = 0;
		int dependencyKeyIndex = 0;
		int depHash = 0;
		int dataIndex = 0;
		int primaryKey = 0;
		int resourceType = 0;

		sObject* data = nullptr;
	};

	class cEntryData
	{
	public:
		 cEntryData() { }
		~cEntryData() { }

		void    fromMemory( sMemory& _memory, sMemory& _extraData );
		sMemory toMemory( void );

	private:
		const int m_entryDataItemPerEntry = 7;

		std::vector<sEntry> m_entries;

	};
}