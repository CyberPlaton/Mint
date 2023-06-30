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


	bool CNode::remove_edge(u64 id)
	{
		bool erased = false;
		if (m_ingoingEdges.lookup(id))
		{
			m_ingoingEdges.remove(id);
			erased = true;
		}
		else if (m_outgoingEdges.lookup(id))
		{
			m_outgoingEdges.remove(id);
			erased = true;
		}
		return erased;
	}

	void CNode::add_outgoing_edge(u64 id, const String& label, f32 weight, CNode* to)
	{
		CEdge edge(id, label, weight, this, to);

		if (!m_outgoingEdges.lookup(id))
		{
			m_outgoingEdges.add(id, edge);

			to->add_ingoing_edge(id, label, weight, this);
		}
		else
		{
			// Adjust weight of the existing edge.
			auto& edge = m_outgoingEdges.get_ref(id);
			
			edge.set_weight(weight);
		}
	}

	void CNode::add_ingoing_edge(u64 id, const String& label, f32 weight, CNode* from)
	{
		CEdge edge(id, label, weight, from, this);

		if (!m_ingoingEdges.lookup(id))
		{
			m_ingoingEdges.add(id, edge);
		}
		else
		{
			// Adjust weight of the existing edge.
			auto& edge = m_ingoingEdges.get_ref(id);

			edge.set_weight(weight);
		}
	}

	CAny& CNode::get_user_data()
	{
		return m_userData;
	}

	CNode::~CNode()
	{
		// Removing all edges involves removing them in target nodes and
		// in nodes that sent them too.
		auto& iedges = m_ingoingEdges.get_all();
		
		for (auto& edge : iedges)
		{
			edge.get_from_node()->remove_edge(edge.get_id());
		}

		auto& oedges = m_outgoingEdges.get_all();

		for (auto& edge : oedges)
		{
			edge.get_to_node()->remove_edge(edge.get_id());
		}
	}

}