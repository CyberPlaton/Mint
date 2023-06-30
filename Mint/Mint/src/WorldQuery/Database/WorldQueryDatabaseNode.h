#ifndef _MINT_WORLD_QUERY_DATABASE_NODE_H_
#define _MINT_WORLD_QUERY_DATABASE_NODE_H_


#include "Common/Common.h"
#include "WorldQueryDatabaseEdge.h"
#include "Utility/STL/Map.h"
#include "Common/Any.h"


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
		~CNode();


		u64 get_id() const;
		void set_id(u64 id);
		
		NodeType get_type() const;
		void set_type(NodeType type);

		String get_label();
		void set_label(const String& label);

		template< typename T >
		void set_user_data(T&& data);
		
		template< typename T >
		void set_user_data(T& data);

		CAny& get_user_data();


		void add_outgoing_edge(u64 id, const String& label, f32 weight, CNode* to);
		void add_ingoing_edge(u64 id, const String& label, f32 weight, CNode* from);
		bool remove_edge(u64 id);

		
	private:
		u64 m_id;
		String m_label;
		NodeType m_type;
		CAny m_userData;

		CMap< CEdge > m_ingoingEdges;
		CMap< CEdge > m_outgoingEdges;

	};

	template< typename T >
	void mint::world::CNode::set_user_data(T& data)
	{
		m_userData.reset();
		m_userData.set(data);
	}

	template< typename T >
	void mint::world::CNode::set_user_data(T&& data)
	{
		m_userData.reset();
		m_userData.set(data);
	}

}


#endif