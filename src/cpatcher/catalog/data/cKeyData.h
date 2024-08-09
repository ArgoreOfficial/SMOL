#pragma once

#include <vector>

#include <sMemory.h>
#include <sObject.h>

namespace smol
{
	

	

	class cBucketData;

	class cKeyData
	{
	public:
		 cKeyData() { }
		~cKeyData() { }

		void    fromMemory( sMemory& _memory, cBucketData* _bucketData );
		sMemory toMemory( void );

	private:

		std::vector<sObject*> m_keys;

	};
}