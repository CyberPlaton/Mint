#ifndef _MINT_WORLD_QUERY_DATABASE_EDGE_H_
#define _MINT_WORLD_QUERY_DATABASE_EDGE_H_


#include "Common/Common.h"


namespace mint::world
{
	class CNode;

	class CEdge
	{
	public:
		CEdge();
		CEdge(u64 id, const String& label, f32 weight, CNode* from, CNode* to);
		CEdge(const CEdge& rh);


		u64 get_id() const;
		void set_id(u64 id);

		String get_label();
		void set_label(const String& label);

		f32 get_weight() const;
		void set_weight(f32 weight);

		CNode* get_from_node() const;
		void set_from_node(CNode* node);

		CNode* get_to_node() const;
		void set_to_node(CNode* node);


	private:
		u64 m_id;
		String m_label;

		f32 m_weight;

		CNode* m_fromNode = nullptr;
		CNode* m_toNode = nullptr;
	};
}


#endif