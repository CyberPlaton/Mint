#include "WorldQueryDatabase.h"

namespace mint::world
{

	mint::Vector< CEdge > CDatabase::query_get_all_object_subject(CNode* object, u64 subject, f32 weight /*= 0.0f*/, LogicalWeightOperator op /*= LogicalWeightOperator_None*/)
	{
		mint::Vector< CEdge > result;

		for (auto& edge : object->m_outgoingEdges.get_all())
		{
			auto h = mint::algorithm::djb_hash(edge.get_label());

			if (h == subject)
			{
				if (weight > 0.0f)
				{
					switch (op)
					{
					case LogicalWeightOperator_Smaller:
					{
						if (edge.get_weight() < weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_Greater:
					{
						if (edge.get_weight() > weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_Equals:
					{
						if (edge.get_weight() == weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_SmallerEqual:
					{
						if (edge.get_weight() <= weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_GreaterEqual:
					{
						if (edge.get_weight() >= weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					default: return mint::Vector< CEdge >{}; // Terminate invalid query.
					}
				}
				else
				{
					mint::algorithm::vector_push_back(result, edge);
				}
			}
		}

		return result;
	}

	bool CDatabase::create_node(u64 id, const String& label, NodeType type)
	{
		if (!m_nodes.lookup(id))
		{
			auto node = m_nodes.add_node(id);

			node->set_id(id);
			node->set_label(label);
			node->set_type(type);


			auto h = mint::algorithm::djb_hash(label);

			m_identifiers[h] = id;

			return true;
		}

		return false;
	}

	bool CDatabase::create_edge(u64 from_node_id, u64 to_node_id, u64 edge_id, const String& label, f32 weight)
	{
		if (m_nodes.lookup(from_node_id))
		{
			auto from = m_nodes.get(from_node_id);
			auto to = m_nodes.get(to_node_id);

			from->add_outgoing_edge(edge_id, label, weight, to);

			return true;
		}

		return false;
	}

	bool CDatabase::create_edge(const String& from_node_label, const String& to_node_label, u64 edge_id, const String& label, f32 weight)
	{
		auto from_node_id = mint::algorithm::djb_hash(from_node_label);
		auto to_node_id = mint::algorithm::djb_hash(to_node_label);

		if (m_identifiers.find(from_node_id) != m_identifiers.end() && 
			m_identifiers.find(to_node_id) != m_identifiers.end())
		{
			from_node_id = m_identifiers[from_node_id];
			to_node_id = m_identifiers[to_node_id];

			if (m_nodes.lookup(from_node_id))
			{
				auto from = m_nodes.get(from_node_id);
				auto to = m_nodes.get(to_node_id);

				from->add_outgoing_edge(edge_id, label, weight, to);

				return true;
			}
		}

		return false;
	}

	bool CDatabase::initialize()
	{
		return m_nodes.initialize(MINT_ENTITY_COUNT_MAX);
	}

	void CDatabase::terminate()
	{
	}

	mint::Vector< mint::world::CEdge > CDatabase::query_get_all_subject_object(u64 subject, CNode* object, f32 weight /*= 0.0f*/, LogicalWeightOperator op /*= LogicalWeightOperator_None*/)
	{
		mint::Vector< CEdge > result;

		for (auto& edge : object->m_ingoingEdges.get_all())
		{
			auto h = mint::algorithm::djb_hash(edge.get_label());

			if (h == subject)
			{
				if (weight > 0.0f)
				{
					switch (op)
					{
					case LogicalWeightOperator_Smaller:
					{
						if (edge.get_weight() < weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_Greater:
					{
						if (edge.get_weight() > weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_Equals:
					{
						if (edge.get_weight() == weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_SmallerEqual:
					{
						if (edge.get_weight() <= weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					case LogicalWeightOperator_GreaterEqual:
					{
						if (edge.get_weight() >= weight) mint::algorithm::vector_push_back(result, edge);
						break;
					}
					default: return mint::Vector< CEdge >{}; // Terminate invalid query.
					}
				}
				else
				{
					mint::algorithm::vector_push_back(result, edge);
				}
			}
		}

		return result;
	}

	void CDatabase::_reset_query_data()
	{
		m_currentQueryObjectIdentifier = 0;
		m_currentQueryObjectLabel.clear();
		m_currentQuerySubjectHash = 0;
		m_currentQuerySubjectLabel.clear();
		m_currentQueryObjectSubject = false;
		m_currentQueryObjectWeight = 0.0f;
		m_currentQueryLogicalWeightOperator = LogicalWeightOperator_None;
	}

	mint::Vector< mint::world::CEdge > CDatabase::testing_run(Vector< SToken >& bytecode)
	{
		for (auto cursor = 0; cursor < bytecode.size(); cursor++)
		{
			SToken& byte = bytecode[cursor];

			switch (byte.m_code)
			{
			case Opcode_SetQueryMode:
			{
				m_currentQueryMode = byte.m_value.cast< QueryMode >();
				break;
			}
			case Opcode_SetQueryResultType:
			{
				m_currentQueryResultType = byte.m_value.cast< ResultType >();
				break;
			}
			case Opcode_SetQueryObject:
			{
				if (byte.m_value.is< String >())
				{
					m_currentQueryObjectLabel = byte.m_value.cast< String >();
				}
				else if (byte.m_value.is< const char* >())
				{
					m_currentQueryObjectLabel = byte.m_value.cast< const char* >();
				}
				else if (byte.m_value.is< u64 >())
				{
					m_currentQueryObjectLabel.clear();
					m_currentQueryObjectIdentifier = byte.m_value.cast< u64 >();

					break;
				}


				auto h = mint::algorithm::djb_hash(m_currentQueryObjectLabel);


				if (m_identifiers.find(h) != m_identifiers.end())
				{
					m_currentQueryObjectIdentifier = m_identifiers[h];
				}

				break;
			}
			case Opcode_SetQuerySubject:
			{
				if (byte.m_value.is< String >())
				{
					m_currentQuerySubjectLabel = byte.m_value.cast< String >();
				}
				else if (byte.m_value.is< const char* >())
				{
					m_currentQuerySubjectLabel = byte.m_value.cast< const char* >();
				}
				else if (byte.m_value.is< u64 >())
				{
					m_currentQuerySubjectLabel.clear();
					m_currentQuerySubjectHash = byte.m_value.cast< u64 >();

					break;
				}

				auto h = mint::algorithm::djb_hash(m_currentQuerySubjectLabel);

				m_currentQuerySubjectHash = h;

				break;
			}
			case Opcode_SetQueryOrderObjectSubject:
			{
				m_currentQueryObjectSubject = byte.m_value.cast< bool >();
				break;
			}
			case Opcode_SetWeightValue:
			{
				m_currentQueryObjectWeight = byte.m_value.cast< f32 >();
				break;
			}
			case Opcode_SetLogicalWeightOperator:
			{
				m_currentQueryLogicalWeightOperator = byte.m_value.cast< LogicalWeightOperator >();
				break;
			}
			default: break;
			}
		}

		mint::Vector< CEdge > query_result;

		switch (m_currentQueryMode)
		{
		case QueryMode_All:
		{
			auto node = m_nodes.get(m_currentQueryObjectIdentifier);

			if (m_currentQueryObjectSubject)
			{
				// Obtain outgoing edges from object to other nodes.
				query_result = query_get_all_object_subject(node, m_currentQuerySubjectHash, m_currentQueryObjectWeight, m_currentQueryLogicalWeightOperator);
			}
			else
			{
				// Obtain ingoing edges from other nodes to node with given subject.
				query_result = query_get_all_subject_object(m_currentQuerySubjectHash, node, m_currentQueryObjectWeight, m_currentQueryLogicalWeightOperator);
			}
			break;
		}
		case QueryMode_Any:
		{
			break;
		}
		case QueryMode_Count:
		{
			break;
		}
		default: {MINT_ASSERT(false, "Invalid operation. Unknown query mode specified!"); break; }
		}


		switch (m_currentQueryResultType)
		{
		case ResultType_Attitude:
		{
			// Filter for edges that point to attitude nodes.
			mint::Vector< CEdge > result;

			for (auto& edge : query_result)
			{
				if (edge.get_to_node()->get_type() == NodeType_Attitude) mint::algorithm::vector_push_back(result, edge);
			}

			return result;
		}
		case ResultType_Relationship:
		{
			// Filter for edges that are relationships, i.e. point to entity nodes.
			mint::Vector< CEdge > result;

			for (auto& edge : query_result)
			{
				if (edge.get_to_node()->get_type() == NodeType_Entity) mint::algorithm::vector_push_back(result, edge);
			}

			return result;

		}
		case ResultType_Membership:
		{
			// Filter for edges that point to membership nodes.
			mint::Vector< CEdge > result;

			for (auto& edge : query_result)
			{
				if (edge.get_to_node()->get_type() == NodeType_Membership) mint::algorithm::vector_push_back(result, edge);
			}

			return result;
		}
		case ResultType_Classification:
		{
			// Filter for edges that point to classification nodes.
			mint::Vector< CEdge > result;

			for (auto& edge : query_result)
			{
				if (edge.get_to_node()->get_type() == NodeType_Classification) mint::algorithm::vector_push_back(result, edge);
			}

			return result;
		}
		case ResultType_Node:
		{
			// Do not filter at all.
			return query_result;
		}
		default: {MINT_ASSERT(false, "Invalid operation. Unknown query result type specified!"); break; }
		}
	}

}