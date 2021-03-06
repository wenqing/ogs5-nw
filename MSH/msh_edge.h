/**************************************************************************
   MSHLib - Object:
   Task:
   Programing:
   08/2005 WW/OK Encapsulation from rf_ele_msh
   last modified
   03/2010 TF added documentation, changed some int, long to size_t
**************************************************************************/
#ifndef msh_edge_INC
#define msh_edge_INC

// MSHLib
#include "msh_node.h"
//------------------------------------------------------------------------
namespace MeshLib
{
/**
 * \brief Class CEdge represent an edge of a finite element mesh.
 *
 * The edge consists of two mesh nodes. Additionally the class
 * stores the middle point as a mesh node
 * (into the vector nodes_of_edges).
 */
class CEdge : public CCore
{
public:
	std::vector<long> connected_elements;     // PCH
	// Methods
	CEdge(size_t Index, bool quadr = false);
	~CEdge();

	// Get functions
	void GetNodes(Math_Group::vec<CNode*>& Nodes)
	{
		for (size_t i = 0; i < 3; i++)
			Nodes[i] = nodes_of_edges[i];
	}

	CNode* GetNode(int l_index)
	{
		return nodes_of_edges[l_index];
	}

	double getLength() const;

#ifndef OGS_ONLY_TH
	int GetJoint() const
	{
		return joint;
	}                                         // PCH

	double GetVelocity(size_t index)
	{
		return velocity[index];
	}                                         // PCH
#endif

	// Set functions
	void SetNodes(Math_Group::vec<CNode*>& Nodes)
	{
		for (int i = 0; i < 3; i++)
			nodes_of_edges[i] = Nodes[i];
	}
	void SetNode(int index, CNode* aNode)
	{
		nodes_of_edges[index] = aNode;
	}
#ifndef OGS_ONLY_TH
	void SetJoint(int i)
	{
		joint = i;
	}                                         // PCH

	void AllocateMeomoryforV()
	{
		if (!velocity) velocity = new double[3];
	}
	void SetVelocity(double* v)
	{
		velocity[0] = v[0];
		velocity[1] = v[1];
		velocity[2] = v[2];
	}                                         // PCH
#endif
	// Operator
	void operator =(CEdge& edg);
	bool operator ==(CEdge& edg);
	// Output
	void Write(std::ostream& osm = std::cout) const;
	//
	//OK
	void SetNormalVector(double* ele_normal_vector, double* normal_vector);
	void GetEdgeVector(double* edge_vector);  //OK
	                                          //OK
	void GetEdgeMidPoint(double* edge_vector);
	size_t GetPatchIndex() const {return patch_index; }
	void SetPatchIndex(size_t index) { patch_index = index; }
private:
	/**
	 * three mesh nodes are stored in this vector,
	 * the third is not really a mesh node, it is the middle point
	 */
	Math_Group::vec<CNode*> nodes_of_edges;
	size_t patch_index;
#ifndef OGS_ONLY_TH
	int joint;                                // PCH
	double* velocity;                         // PCH
#endif

	friend class CElem;
};
}                                                 // namespace MeshLib
#endif
