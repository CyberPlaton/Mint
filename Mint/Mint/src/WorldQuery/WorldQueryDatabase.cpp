#include "WorldQueryDatabase.h"

namespace mint::world
{

	mint::Vector< CEdge > CDatabase::query_get_all_object_subject_edges(const String& object, const String& subject, f32 weight /*= 0.0f*/,
		LogicalWeightOperator op /*= LogicalWeightOperator_None*/, CWorldQueryDatabaseFilter* filter /*= nullptr*/)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CDatabase::query_get_all_object_subject");

		// Setup database query.
		_reset_query_data();
		m_currentQueryResultType = ResultType_Edge;
		m_currentQueryMode = QueryMode_All;
		m_currentQueryObjectSubject = true;
		m_currentQueryObjectWeight = weight;
		m_currentQueryLogicalWeightOperator = op;
		m_currentQueryFilter = filter;

		mint::Vector< CEdge > result;

		if (!_set_query_object_and_subject(object, subject))
		{
			return result;
		}


		for (auto& edge : m_currentQueryObjectNode->m_outgoingEdges.get_all())
		{
			if (m_currentQueryFilter)
			{
				if (!(m_currentQueryFilter->does_edge_pass_filter(&edge) &&
					  m_currentQueryFilter->does_from_node_pass_filter(edge.get_from_node()) &&
					  m_currentQueryFilter->does_to_node_pass_filter(edge.get_to_node())))
				{
					// Edge or nodes did not pass the filter, ignore and continue.
					continue;
				}
			}

			auto h = mint::algorithm::djb_hash(edge.get_label());
			auto any_hash = mint::algorithm::djb_hash("*");

			if (h == m_currentQuerySubjectHash || any_hash == m_currentQuerySubjectHash)
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
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CDatabase::create_node");

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
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CDatabase::create_edge");

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
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CDatabase::create_edge");

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

	mint::Vector< mint::world::CEdge > CDatabase::query_get_all_subject_object_edges(const String& subject, const String& object, f32 weight /*= 0.0f*/,
		LogicalWeightOperator op /*= LogicalWeightOperator_None*/, CWorldQueryDatabaseFilter* filter /*= nullptr*/)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CDatabase::query_get_all_subject_object");

		// Setup database query.
		_reset_query_data();
		m_currentQueryResultType = ResultType_Edge;
		m_currentQueryMode = QueryMode_All;
		m_currentQueryObjectSubject = false;
		m_currentQueryObjectWeight = weight;
		m_currentQueryLogicalWeightOperator = op;
		m_currentQueryFilter = filter;


		mint::Vector< CEdge > result;


		if (!_set_query_object_and_subject(object, subject))
		{
			return result;
		}


		for (auto& edge : m_currentQueryObjectNode->m_ingoingEdges.get_all())
		{
			if (m_currentQueryFilter)
			{
				if (!(m_currentQueryFilter->does_edge_pass_filter(&edge) &&
					m_currentQueryFilter->does_from_node_pass_filter(edge.get_from_node()) &&
					m_currentQueryFilter->does_to_node_pass_filter(edge.get_to_node())))
				{
					// Edge or nodes did not pass the filter, ignore and continue.
					continue;
				}
			}


			auto h = mint::algorithm::djb_hash(edge.get_label());
			auto any_hash = mint::algorithm::djb_hash("*");

			if (h == m_currentQuerySubjectHash || m_currentQuerySubjectHash == any_hash)
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
		m_currentQueryObjectNode = nullptr;
		m_currentQueryObjectIdentifier = 0;
		m_currentQueryObjectLabel.clear();
		m_currentQuerySubjectHash = 0;
		m_currentQuerySubjectLabel.clear();
		m_currentQueryObjectSubject = false;
		m_currentQueryObjectWeight = 0.0f;
		m_currentQueryLogicalWeightOperator = LogicalWeightOperator_None;
		m_currentQueryFilter = nullptr;
	}


	bool CDatabase::_set_query_object_and_subject(const String& object, const String& subject)
	{
		bool result = false;
		
		// Object.
		auto oh = mint::algorithm::djb_hash(object);
		auto sh = mint::algorithm::djb_hash(subject);


		if (m_identifiers.find(oh) != m_identifiers.end())
		{
			m_currentQueryObjectIdentifier = m_identifiers[oh];
			m_currentQueryObjectLabel = object;

			m_currentQueryObjectNode = m_nodes.get(m_currentQueryObjectIdentifier);

			result = (m_currentQueryObjectNode != nullptr);
		}
		else
		{
			MINT_ASSERT(false, "Invalid operation. Could not locate object in the database!");
		}

		m_currentQuerySubjectHash = sh;
		m_currentQuerySubjectLabel = subject;

		return result;
	}

}