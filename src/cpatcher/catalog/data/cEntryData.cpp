#include "cEntryData.h"

#include <sObject.h>
#include <json.hpp>

void smol::cEntryData::fromMemory( sMemory& _memory, sMemory& _extraData )
{
	const int bytesPerInt32 = sizeof( int );

	int numEntries = _memory.at<int32_t>( 0 );
	m_entries.resize( numEntries );

	for ( int i = 0; i < numEntries; i++ )
	{
		int index = bytesPerInt32 + i * ( bytesPerInt32 * m_entryDataItemPerEntry );
		m_entries[ i ].internalId = _memory.at<int32_t>( index );
		index += bytesPerInt32;
		m_entries[ i ].providerIndex = _memory.at<int32_t>( index );
		index += bytesPerInt32;
		m_entries[ i ].dependencyKeyIndex = _memory.at<int32_t>( index );
		index += bytesPerInt32;
		m_entries[ i ].depHash = _memory.at<int32_t>( index );
		index += bytesPerInt32;
		m_entries[ i ].dataIndex = _memory.at<int32_t>( index );
		index += bytesPerInt32;
		m_entries[ i ].primaryKey = _memory.at<int32_t>( index );
		index += bytesPerInt32;
		m_entries[ i ].resourceType = _memory.at<int32_t>( index );

		// object data = dataIndex < 0 ? null : SerializationUtilities.ReadObjectFromByteArray( extraData, dataIndex );
		if ( m_entries[ i ].dataIndex >= 0 )
		{
			sObject* data = sObject::fromMemory( _extraData, m_entries[ i ].dataIndex );
			m_entries[ i ].data = data;


			/// hardcoded sherman crc test
			if ( data->type == eObjectType::JsonObject )
			{
				// parse json
				nlohmann::json jdata = nlohmann::json::parse( data->val.jsonObject->jsonText );
				std::string hash = jdata.at( "m_Hash" );
				if ( hash == "eb57745bbeb40b4e7fe99937f4687023" )
				{
					data->val.jsonObject->toMemory();
			/*
					std::string bundleName = jdata[ "m_BundleName" ];

					printf( "Editing sherman json\n" );
					printf( "%s\n", jdata.dump().c_str() );
					jdata[ "m_UseCrcForCachedBundles" ] = false;
					printf( "%s\n", jdata.dump().c_str() );
			*/
				}
			}
		}
		
		
	}

	printf( "Loaded %i Entries\n", numEntries );
}

smol::sMemory smol::cEntryData::toMemory( void )
{
	return sMemory();
}
