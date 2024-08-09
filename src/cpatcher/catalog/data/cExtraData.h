#pragma once

#include "iCatalogData.h"

#include <vector>

namespace smol
{
	class cExtraData
	{
	public:
		cExtraData() { }
		~cExtraData() { }

		void    fromMemory( sMemory& _memory );
		sMemory toMemory( void );

	private:

	};
}