#ifndef _MAML_NODE_H_
#define _MAML_NODE_H_


#include "Common/Property.h"

#include "../STL/Map.h"


namespace maml
{
	class CDocument;

	struct SNode
	{
		SNode();
		~SNode();


		CDocument* m_document;

		mint::u64 m_uuid;

		mint::String m_name;

		mint::CMap< SProperty > m_properties;

		SNode* m_parent;

		mint::Vector< SNode* > m_children;
	};

}

#endif