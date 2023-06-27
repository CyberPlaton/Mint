#include "WorldQueryDatabaseEdge.h"


namespace mint::world
{

	CEdge::CEdge(u64 id, const String& label, f32 weight, CNode* from, CNode* to) :
		m_id(id), m_label(label), m_weight(weight), m_fromNode(from), m_toNode(to)
	{
	}

	CEdge::CEdge() :
		m_id(0), m_label(""), m_weight(0.0f), m_fromNode(nullptr), m_toNode(nullptr)
	{
	}

	CEdge::CEdge(const CEdge& rh)
	{
		m_id = rh.m_id;
		m_label = rh.m_label;
		m_weight = rh.m_weight;
		m_fromNode = rh.m_fromNode;
		m_toNode = rh.m_toNode;
	}

	glm::u64 CEdge::get_id() const
	{
		return m_id;
	}

	void CEdge::set_id(u64 id)
	{
		m_id = id;
	}

	String CEdge::get_label()
	{
		return m_label;
	}

	void CEdge::set_label(const String& label)
	{
		m_label = label;
	}

	glm::f32 CEdge::get_weight() const
	{
		return m_weight;
	}

	void CEdge::set_weight(f32 weight)
	{
		m_weight = weight;
	}

	mint::world::CNode* CEdge::get_from_node() const
	{
		return m_fromNode;
	}

	void CEdge::set_from_node(CNode* node)
	{
		m_fromNode = node;
	}

	mint::world::CNode* CEdge::get_to_node() const
	{
		return m_toNode;
	}

	void CEdge::set_to_node(CNode* node)
	{
		m_toNode = node;
	}

}