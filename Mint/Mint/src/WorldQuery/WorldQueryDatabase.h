#ifndef _MINT_WORLD_QUERY_DATABASE_H_
#define _MINT_WORLD_QUERY_DATABASE_H_


#include "Database/WorldQueryDatabaseNode.h"
#include "Database/WorldQueryDatabaseQueryMode.h"
#include "Database/WorldQueryDatabaseQueryResultType.h"
#include "Common/WorldQueryFilter.h"

#include "Common/Any.h"
#include "Utility/Logging/Logging.h"
#include "Utility/STL/BinarySearchTree.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint::world
{

	enum Opcode
	{
		Opcode_None = 0,
		Opcode_SetQueryMode,				// All, Any etc.
		Opcode_SetQueryResultType,			// Relationship, Node etc.
		Opcode_SetQueryOrderObjectSubject,	// Subject-Object (false) or Object-Subject (true).
		Opcode_SetQueryObject,				// i.e. "x" that is the node identifier.
		Opcode_SetQuerySubject,				// i.e. "Likes".
		Opcode_SetWeightValue,				// i.e. "1.2f".
		Opcode_SetLogicalWeightOperator		// i.e. 0 for equals etc.
	};

	enum LogicalWeightOperator
	{
		LogicalWeightOperator_None = 0,
		LogicalWeightOperator_Smaller,
		LogicalWeightOperator_Greater,
		LogicalWeightOperator_Equals,
		LogicalWeightOperator_SmallerEqual,
		LogicalWeightOperator_GreaterEqual
	};


	struct SToken
	{
		Opcode m_code;
		CAny m_value;
	};


	class CDatabase
	{
	public:
		CDatabase();

		bool initialize(u64 expected_object_count, u64 alignment = 0);

		void terminate();


		bool create_node(u64 id, const String& label, NodeType type);

		template< typename T >
		bool create_node(u64 id, const String& label, NodeType type, T& user_data);

		template< typename T >
		bool create_node(u64 id, const String& label, NodeType type, T&& user_data);

		bool remove_node(u64 id);


		bool create_edge(u64 from_node_id, u64 to_node_id, u64 edge_id, const String& label, f32 weight);

		bool create_edge(const String& from_node_label, const String& to_node_label, u64 edge_id, const String& label, f32 weight);


		/// @brief Remove edge by identifier. Slow: involves searched for the edge through nodes.
		/// @param edge_id Unique identifier of the edge.
		/// @return True if removed.
		bool remove_edge(u64 edge_id);


		/// @brief Remove edge by identifier. Faster: as node is given we remove the edge directly.
		/// @param node_id Unique identifier of the node.
		/// @param edge_id Unique identifier of the edge.
		/// @return True if removed.
		bool remove_edge(u64 node_id, u64 edge_id);


		/// @brief Query the database for a node and its associated data. 
		///		   This function queries for outgoing edges that are labeled 
		///		   as the Subject from the Object to other nodes.
		///		   	   
		/// @param object Label of the Node of interest. Nodes are uniquely named.
		/// @param subject Label of the Subject of interest describing a relationship, i.e. "Like".
		/// @param weight Optional. Filter weight.
		/// @param op Optional. Logical operator for the filter weight.
		/// @param filter Optional. User defined filter for edges found. The filter can check for specific user data etc.
		/// @return Array of edges fulfilling the requirements and filters.
		Vector< CEdge > query_get_all_object_subject_edges(const String& object, const String& subject, f32 weight = 0.0f, LogicalWeightOperator op = LogicalWeightOperator_None, CWorldQueryDatabaseFilter* filter = nullptr);


		/// @brief Query the database for a node and its associated data. 
		///		   This function queries for ingoing edges that are labeled 
		///		   as the Subject from other nodes to the Object node.
		Vector< CEdge > query_get_all_subject_object_edges(const String& subject, const String& object, f32 weight = 0.0f, LogicalWeightOperator op = LogicalWeightOperator_None, CWorldQueryDatabaseFilter* filter = nullptr);


	private:
		/// @brief BST of nodes, findable with unique id.
		CBinarySearchTree< CNode > m_nodes;

		/// @brief Mapping object (node) labels to unique ids. Node can then be found through id in BST.
		std::unordered_map< u64, u64 > m_identifiers;

		/// @brief Filter to apply on nodes or edges that are being queried.
		CWorldQueryDatabaseFilter* m_currentQueryFilter = nullptr;

		ResultType m_currentQueryResultType = ResultType_None;
		QueryMode m_currentQueryMode = QueryMode_None;
		
		/// @brief Referring to the main query object, i.e. the node of interest.
		CNode* m_currentQueryObjectNode = nullptr;
		u64 m_currentQueryObjectIdentifier = 0;
		String m_currentQueryObjectLabel;
		

		/// @brief Referring to the secondary query subject, i.e. the relationship of interest related to main query object.
		u64 m_currentQuerySubjectHash = 0;
		String m_currentQuerySubjectLabel;

		/// @brief Ordering mode, basically whether to query for outgoing or ingoing edges. 
		bool m_currentQueryObjectSubject = false;

		/// @brief Weight filter for edges.
		f32 m_currentQueryObjectWeight = 0.0f;

		/// @brief Logical operator for edges filtering.
		LogicalWeightOperator m_currentQueryLogicalWeightOperator = LogicalWeightOperator_None;



	private:
		void _reset_query_data();
	
		bool _set_query_object_and_subject(const String& object, const String& subject);

	};

	template< typename T >
	bool mint::world::CDatabase::create_node(u64 id, const String& label, NodeType type, T&& user_data)
	{
		T& _d = user_data;

		return create_node(id, label, type, _d);
	}

	template< typename T >
	bool mint::world::CDatabase::create_node(u64 id, const String& label, NodeType type, T& user_data)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CDatabase::create_node");

		if (!m_nodes.lookup(id))
		{
			auto node = m_nodes.add_node(id);

			node->set_id(id);
			node->set_label(label);
			node->set_type(type);
			node->set_user_data(user_data);

			auto h = mint::algorithm::djb_hash(label);

			m_identifiers[h] = id;

			return true;
		}

		return false;
	}

}


#endif