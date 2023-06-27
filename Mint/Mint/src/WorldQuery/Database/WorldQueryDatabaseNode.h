#ifndef _MINT_WORLD_QUERY_DATABASE_NODE_H_
#define _MINT_WORLD_QUERY_DATABASE_NODE_H_


#include "Common/Common.h"
#include "WorldQueryDatabaseEdge.h"
#include "Utility/STL/Map.h"


namespace mint::world
{
	class CDatabase;

	enum NodeType
	{
		NodeType_None = 0,
		NodeType_Entity,
		NodeType_Membership,
		NodeType_Attitude,
		NodeType_Classification,
	};


	class CNode
	{
		friend class CDatabase;

	public:
		CNode();
		CNode(NodeType type, const String& label, u64 id);


		u64 get_id() const;
		void set_id(u64 id);
		
		NodeType get_type() const;
		void set_type(NodeType type);

		String get_label();
		void set_label(const String& label);

		void add_outgoing_edge(u64 id, const String& label, f32 weight, CNode* to);
		void add_ingoing_edge(u64 id, const String& label, f32 weight, CNode* from);
		void remove_edge(u64 id);

		
	private:
		u64 m_id;
		String m_label;
		NodeType m_type;

		CMap< CEdge > m_ingoingEdges;
		CMap< CEdge > m_outgoingEdges;

	};

}


#endif