/**
 * \file msh_mesh.h
 */

/**************************************************************************
 MSHLib - Object:
 Task:
 Programing:
 08/2005 WW/OK Encapsulation from rf_ele_msh
 last modified
 **************************************************************************/
#ifndef msh_mesh_INC
#define msh_mesh_INC

// C++
#include "Configure.h"
#include <string>

/** depreciated includes of GEOLib */
#include "geo_lib.h"
//#include "geo_pnt.h"
#include "geo_sfc.h"
#include "geo_vol.h"

// GEOLIB
#include "GEOObjects.h"
#include "Point.h"
#include "Polyline.h"
#include "Surface.h"

// MSHLib
#include "MSHEnums.h"                             // KR 2010/11/15
#include "MeshNodesAlongPolyline.h"

// FileIO
#include "MeshIO/OGSMeshIO.h"

#include "msh_elem.h"

class RandomWalk;
class CFluidMomentum;

class Problem;

#ifdef NEW_EQS                                    //1.11.2007 WW
namespace Math_Group {class SparseTable;}
using Math_Group::SparseTable;
#endif

//------------------------------------------------------------------------
namespace MeshLib
{

#ifndef NON_GEO
   /*!
      Class to handle topologic relationship among grids
      Designed by WW
      First version:    13.05.2009
   */
   class GridsTopo
   {
      private:
         long *local_indices;                     // of border nodes in local node array of a grid
         //double *comm_data;   // data for communication
         std::string neighbor_name;
         long bnodes;
         friend class CFEMesh;
         friend class ::CRFProcess;
      public:
         GridsTopo(std::istream &in, std::string sec_name);
         std::string getNeighbor_Name() const {return neighbor_name;}
         long *getBorderNodeIndicies() const {return local_indices;}
         long getBorderNodeNumber() const {return bnodes;}

         //void Write(std::ostream &os=cout);
         ~GridsTopo();
   };
#endif                                         //#ifndef NON_GEO

   //------------------------------------------------------------------------
   // Class definition
   class CFEMesh
   {
      public:

         /// Constructor using geometric information.
         CFEMesh(GEOLIB::GEOObjects* geo_obj = NULL, std::string* unique_name = NULL);

         /// Copy-Constructor.
         CFEMesh(const CFEMesh* mesh);

         /// Destructor
         ~CFEMesh();

#ifndef NON_GEO
         GEOLIB::GEOObjects* getGEOObjects () const { return _geo_obj; }

         std::string* getProjectName () const { return _geo_name; }
#endif                                      //#ifndef NON_GEO

         /**
          * sets the value for element type
          * @param ele_type
          */
         void setElementType (MshElemType::type ele_type);

         /**
          * set the number of mesh layer
          * @param n_msh_layer
          */
         void setNumberOfMeshLayers (size_t n_msh_layer);

         /**
          * returns the number of mesh layers
          * @return the number of mesh layers
          */
         size_t getNumberOfMeshLayers () const;   // TF

         /**
          *
          * @return
          */
         bool hasCrossSection() const;

         size_t getNumberOfLines () const;
         size_t getNumberOfQuads () const;
         size_t getNumberOfHexs () const;
         size_t getNumberOfTris () const;
         size_t getNumberOfTets () const;
         size_t getNumberOfPrisms () const;
         double getMinEdgeLength () const;
         /**
          * do not use this method REMOVE CANDIDATE
          * @param val
          */
         void setMinEdgeLength (double val)       // TF temporary
         {
            _min_edge_length = val;
         }

         void Read(std::ifstream*);

         friend class FileIO::OGSMeshIO;

         std::ios::pos_type GMSReadTIN(std::ifstream*);
         //
         void ConstructGrid();
         void GenerateHighOrderNodes();
         //
//         void RenumberNodesForGlobalAssembly();
         // For number of nodes
         int GetMaxElementDim() const
         {
            return max_ele_dim;
         }
         void SwitchOnQuadraticNodes(bool quad)
         {
            useQuadratic = quad;
         }
         bool getOrder() const
         {
            return useQuadratic;
         }
         bool isAxisymmetry() const
         {
            return _axisymmetry;
         }
         // Get number of nodes
                                                  //CMCD int to long
         size_t GetNodesNumber(const bool quadr) const
         {
            if (quadr)
               return NodesNumber_Quadratic;
            else
               return NodesNumber_Linear;
         }

         size_t NodesInUsage() const
         {
            if (useQuadratic)
               return NodesNumber_Quadratic;
            else
               return NodesNumber_Linear;
         }

         void InitialNodesNumber()                //WW
         {
            NodesNumber_Quadratic = NodesNumber_Linear = nod_vector.size();
         }

         void setNumberOfElementsFromElementsVectorSize ()
         {
        	 NodesNumber_Linear = nod_vector.size();
         }

#ifndef NON_GEO                             //WW
         /**
          * @{
          * */

         /**
          * \brief depreciated method - uses old surface class
          */
         void GetNODOnPLY_XY(CGLPolyline*m_ply, std::vector<long>&msh_nod_vector);
         //	/**
         //	 * \brief depreciated method - uses old surface class
         //	 */
         void GetNODOnSFC_Vertical(Surface*m_sfc, std::vector<long>&msh_nod_vector);

         /**
          * \brief depreciated method
          */
         void CreateLineELEFromPLY(CGLPolyline*);
         /**
          * \brief depreciated method
          */
         void CreateLayerPolylines(CGLPolyline*); //OK

         // GEO-SFC
         /**
          * \brief depreciated method
          */
         void GetNODOnSFC(Surface*, std::vector<long>&);
         /**
          * \brief depreciated method
          */
         void GetNODOnSFC_PLY(Surface const*, std::vector<long>&) const;
         /**
          * \brief depreciated method
          */
         void GetNODOnSFC_PLY_XY(Surface*m_sfc, std::vector<long>& msh_nod_vector,
            bool givenNodesOnSurface = false);    // givenNodeOnSurface by WW
         /**
          * \brief depreciated method
          */
                                                  // 02.2009/OK
         void GetNODOnSFC_PLY_Z(Surface*, std::vector<long>&);

         /**
          * \brief depreciated method
          */
         void GetNODOnSFC_TIN(Surface*, std::vector<long>&);
         /**
          * \brief deprecated method
          */
         void GetNodesOnCylindricalSurface(Surface*m_sfc, std::vector<long>& NodesS);
         /**
          * \brief depreciated method
          */
         void CreateQuadELEFromSFC(Surface*);

         /**
          * GetNODOnPNT searchs the nearest node to the geometric point
          * */
         long GetNODOnPNT(const GEOLIB::Point* const pnt) const;
         /**
          * GetNearestELEOnPNT searchs the nearest element (gravity center)
          * to the geometric point
          * */
         long GetNearestELEOnPNT(const GEOLIB::Point* const pnt) const;

         /**
          * GetNODOnPLY search the nearest nodes along the Polyline object
          * @param ply constant pointer to a constant Polyline object
          * @param msh_nod_vector the mesh node indices are saved in this vector
          * */
         void GetNODOnPLY(const GEOLIB::Polyline* const ply,
            std::vector<size_t>& msh_nod_vector);

         /**
          *
          * @param ply
          * @return
          */
         const MeshNodesAlongPolyline& GetMeshNodesAlongPolyline(const GEOLIB::Polyline* const ply);

         /**
          *
          * @param ply
          * @param points
          */
         void getPointsForInterpolationAlongPolyline (const GEOLIB::Polyline* const ply, std::vector<double>& points);

         /**
          * GetNODOnPLY search the nearest nodes to the Polyline
          * */
         void GetNODOnPLY(const GEOLIB::Polyline* const ply, std::vector<long>& msh_nod_vector);

         /**
          * \brief gives the indices of CElement elements, which have an edge
          * in common with the polyline.
          */
         void GetELEOnPLY(const GEOLIB::Polyline*, std::vector<size_t>&);

         /**
          * \brief gives the indices of nodes, which are contained in the surface
          */
         void GetNODOnSFC(const GEOLIB::Surface* sfc, std::vector<size_t>& msh_nod_vector) const;

         /** @} */ // close doxygen group
#endif                                      //WW #ifndef NON_GEO
//#ifndef NON_GEO                             //  WW
//         /**
//          * Store border nodes among different grids.
//          */
//         std::vector<GridsTopo*> grid_neighbors;
//         friend class ::Problem;
//#endif
         //....................................................................
         // QUAD->HEX
         void CreateHexELEFromQuad(int, double);
         // QUAD->LINE
         void CreateLineELEFromQuad(int, double, int);
         void SetActiveElements(std::vector<long>&);
                                                  //MB
         void SetMSHPart(std::vector<long>&, long);
         bool NodeExists(size_t node);
         // LINE->LINE
         void AppendLineELE();

#ifndef NON_GEO
         // TRI->PRIS
         void CreatePriELEFromTri(int, double);

#endif

         // Coordinate system
         int GetCoordinateFlag() const { return coordinate_system; };
         void FillTransformMatrix();

         /**
          * returns the vector storing pointers to all nodes (class CNode) of the mesh
          * @return
          */
         const std::vector<MeshLib::CNode*>& getNodeVector () const { return nod_vector; }

         // All nodes - should be private!!!
         std::vector<MeshLib::CNode*> nod_vector;

         // All edges
         std::vector<MeshLib::CEdge*> edge_vector;
         // All surface faces
         std::vector<MeshLib::CElem*> face_vector;
         // All surface normal
         std::vector<double*> face_normal;        //YD

         const std::vector<MeshLib::CElem*>& getElementVector () const { return ele_vector; }
         /**
          * all elements stored in this vector
          * */
         std::vector<MeshLib::CElem*> ele_vector;
         // Nodes in usage
         // To record eqs_index->global node index
         std::vector<long> Eqs2Global_NodeIndex;

                                                  //OK
         void PrismRefine(int Layer, int subdivision);

         void ConnectedNodes(bool quadratic) const;
                                                  //WW
         void ConnectedElements2Node(bool quadratic = false);
                                                  //OK
         std::vector<std::string> mat_names_vector;
         void DefineMobileNodes(CRFProcess*);     //OK
         void FaceNormal();                       // YD
         void SetNODPatchAreas();                 //OK4310
         void SetNetworkIntersectionNodes();      //OK4319->PCH
#ifndef NON_PROCESS                         // 05.03.2010 WW
#ifdef NEW_EQS                              // 1.11.2007 WW
         // Compute the graph of the sparse matrix related to this mesh. 1.11.2007 WW
         void CreateSparseTable();
         // Get the sparse graph   1.11.2007 WW
         SparseTable *GetSparseTable(bool quad = false)
            const {if(!quad) return sparse_graph; else return sparse_graph_H;}
#endif
#endif

         std::string pcs_name;
         std::string geo_name;                    //MB
         std::string geo_type_name;               //OK10_4310

         int max_mmp_groups;                      //OKCC
         int msh_max_dim;

         RandomWalk* PT;                          // PCH

         CFluidMomentum* fm_pcs;                  // by PCH

         /// Import MODFlow grid. 10.2009 WW
         void ImportMODFlowGrid(std::string const & fname);
         /// Convert raster cells into grid. 12.2009 WW
         void ConvertShapeCells(std::string const & fname);
#ifdef USE_HydSysMshGen
         // Be activated if it is still needed.
         // WW
         /// Generate Column-surface grid system for the modeling of surafce-subsuface coupled processes
                                                  //15.05.2009. WW // removed useless const TF
         void HydroSysMeshGenerator(std::string fname, int nlayers, double thickness, int mapping);

#endif
         void MarkInterface_mHM_Hydro_3D();       //07.06.2010. WW
         void mHM2NeumannBC();
         /// Comptute \int {f} a dA on top surface.
         void TopSurfaceIntegration();

      private:
         // private attributes
         /**
          * reference to object of class GEOObject, that manages the geometry data
          */
         GEOLIB::GEOObjects* _geo_obj;
         /**
          * identifier for geometry
          */
         std::string* _geo_name;
         std::vector<MeshLib::MeshNodesAlongPolyline> _mesh_nodes_along_polylines;

         MshElemType::type _ele_type;
         size_t _n_msh_layer;                     //OK
         bool _cross_section;
         size_t _msh_n_lines;
         size_t _msh_n_quads;
         size_t _msh_n_hexs;
         size_t _msh_n_tris;
         size_t _msh_n_tets;
         size_t _msh_n_prisms;

         /**
          * method initializes the minimal edge length that is used in search algorithms
          */
         void computeMinEdgeLength();
         double _min_edge_length;                 //TK

         // Process friends
         friend class ::CRFProcess;

         size_t NodesNumber_Linear;
         size_t NodesNumber_Quadratic;
         bool useQuadratic;
         bool _axisymmetry;
         bool top_surface_checked;                // 07.06.2010.  WW

         // Coordinate indicator
         // 1:  X component only
         // 12: Y component only
         // 13: Z component only
         // 2:  X, Y component
         // 23:  X, Z component
         // 3:  X, Y, Z component
         int coordinate_system;
         bool has_multi_dim_ele;
         int max_ele_dim;
         int map_counter;                         //21.01.2009 WW

         bool mapping_check;                      //23.01.2009 WW
         /// Import shape file. 16.03.2026. WW
         size_t ncols, nrows;
         /// (x_0, y_0): coordinate of the left down corner
         double x0, y0, csize, ndata_v;
         std::vector<double>  zz;                 //Elevation
         inline void ReadShapeFile(std::string const & fname);
         // 03.2010. WW
         inline void Precipitation2NeumannBC(std::string const & fname, std::string const & ofname, double ratio = 0.8);

#ifndef NON_GEO                             //  WW
         /// Store border nodes among different grids.
         std::vector<GridsTopo*> grid_neighbors;
         friend class ::Problem;
#endif                                      // #ifndef NON_GEO
         //
         // Sparse graph of this mesh. 1.11.2007 WW
#ifdef NEW_EQS
         SparseTable *sparse_graph;
         SparseTable *sparse_graph_H;             // For high order interpolation
#endif

#ifndef NON_GEO                             //WW
         void CreateLineElementsFromMarkedEdges(CFEMesh*m_msh_ply,
            std::vector<long> &ele_vector_at_ply);//NW
#endif                                      // #ifndef NON_GEO //WW
   };

}                                                 // namespace MeshLib
#endif
