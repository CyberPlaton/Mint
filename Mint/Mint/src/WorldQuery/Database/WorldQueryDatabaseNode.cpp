#include "WorldQueryDatabaseNode.h"


namespace mint::world
{

	CNode::CNode() :
		m_id(0), m_label(""), m_type(NodeType_None)
	{
	}

	CNode::CNode(NodeType type, const String& label, u64 id) :
		m_id(id), m_label(label), m_type(type)
	{
	}

	glm::u64 CNode::get_id() const
	{
		return m_id;
	}

	String CNode::get_label()
	{
		return m_label;
	}

	NodeType CNode::get_type() const
	{
		return m_type;
	}

	void CNode::set_id(u64 id)
	{
		m_id = id;
	}

	void CNode::set_type(NodeType type)
	{
		m_type = type;
	}

	void CNode::set_label(const String& label)
	{
		m_label = label;
	}


	void CNode::remove_edge(u64 id)
	{
		if (m_ingoingEdges.lookup(id))
		{
			m_ingoingEdges.remove(id);
		}
		else if (m_outgoingEdges.lookup(id))
		{
			m_outgoingEdges.remove(id);
		}
	}

	void CNode::add_outgoing_edge(u64 id, const String& label, f32 weight, CNode* to)
	{
		CEdge edge(id, label, weight, this, to);

		if (!m_outgoingEdges.lookup(id))
		{
			m_outgoingEdges.add(id, edge);

			to->add_ingoing_edge(id, label, weight, this);
		}
	}

	void CNode::add_ingoing_edge(u64 id, const String& label, f32 weight, CNode* from)
	{
		CEdge edge(id, label, weight, from, this);

		if (!m_ingoingEdges.lookup(id))
		{
			m_ingoingEdges.add(id, edge);
		}
	}

	CAny& CNode::get_user_data()
	{
		return m_userData;
	}

}