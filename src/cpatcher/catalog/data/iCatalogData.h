#pragma once

#include <sMemory.h>

namespace smol
{
	class iCatalogData
	{
	public:
		virtual ~iCatalogData() { }
		
		virtual void fromMemory( sMemory& _memory ) = 0;
		virtual sMemory toMemory() = 0;

	private:
		
	};
}