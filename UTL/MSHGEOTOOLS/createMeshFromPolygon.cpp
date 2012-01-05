/*
 * createMeshFromPolygon.cpp
 *
 *  Created on: Jan 4, 2012
 *      Author: TF
 */

// STL
#include <vector>
#include <string>

// FileIO
#include "OGSIOVer4.h"
#include "XmlIO/XmlGmlInterface.h"

// FileIO/GeoIO
#include "GeoIO/Gmsh2GeoIO.h"
// FileIO/MeshIO
#include "GMSHInterface.h"

// GEO
#include "GEOObjects.h"
#include "Point.h"
#include "PolylineVec.h"
#include "ProjectData.h"

int main (int argc, char* argv[])
{
	if (argc < 5) {
		std::cout << "Usage: " << argv[0] << " --geometry ogs_geometry --polyline-id id" << std::endl;
		return -1;
	}

	// *** read geometry
	std::string tmp(argv[1]);
	if (tmp.find ("--geometry") == std::string::npos)
	{
		std::cout << "could not extract geometry file name" << std::endl;
		return -1;
	}

	GEOLIB::GEOObjects* geo (new GEOLIB::GEOObjects);
	tmp = argv[2];
	std::string unique_name;
	FileIO::readGLIFileV4(tmp, geo, unique_name);

	// *** parse polyline id
	tmp = argv[3];
	if (tmp.find ("--polyline-id") == std::string::npos)
	{
		std::cout << "expecting option --polyline-id" << std::endl;
		return -1;
	}
	size_t polygon_id (atoi (argv[4]));

	// *** get Polygon
	const std::vector<GEOLIB::Polyline*>* plys (geo->getPolylineVec (unique_name));
	if (!plys)
	{
		std::cout << "could not get vector of polylines" << std::endl;
		delete geo;
		return -1;
	}

	// *** extract necessary information for gmsh file
	if (! (*plys)[polygon_id]->isClosed()) {
		std::cout << "polyline with id " << polygon_id << " is not a polygon" << std::endl;
		return -1;
	}
	std::vector<GEOLIB::Point*> *new_ply_pnts(new std::vector<GEOLIB::Point*>);
	std::vector<GEOLIB::Polyline*> *new_plys(new std::vector<GEOLIB::Polyline*>);
	new_plys->push_back(new GEOLIB::Polyline(*new_ply_pnts));

	for (size_t k(0); k<(*plys)[polygon_id]->getNumberOfPoints()-1; k++) {
		new_ply_pnts->push_back(new GEOLIB::Point(*((*plys)[polygon_id]->getPoint(k))));
		(*new_plys)[0]->addPoint(k);
	}
	(*new_plys)[0]->addPoint(0);

	std::string tmp_name("PolygonForSurface");
	tmp_name += number2str(polygon_id);
	geo->addPointVec(new_ply_pnts, tmp_name);
	geo->addPolylineVec(new_plys, tmp_name);

	std::string ply_name;
	geo->getPolylineVecObj(unique_name)->getNameOfElementByID(polygon_id, ply_name);

	std::string base_fname("Surface-"+ply_name); //number2str(polygon_id));
	std::string gmsh_geo_fname(base_fname+".geo");
	std::string gmsh_msh_fname(base_fname+".msh");
	FileIO::GMSHInterface gmsh_io(gmsh_geo_fname);
	gmsh_io.writeGMSHInputFile(tmp_name, *geo, false, false);

	if (system(NULL) != 0) { // command processor available
		std::string gmsh_command("gmsh -2 ");
		gmsh_command += gmsh_geo_fname;
		gmsh_command += " -o " + gmsh_msh_fname;
		system(gmsh_command.c_str());
	}

	// *** read GMSH mesh
	FileIO::Gmsh2GeoIO::loadMeshAsGeometry(gmsh_msh_fname, geo);

	ProjectData* project_data (new ProjectData);
	project_data->setGEOObjects (geo);
	XmlGmlInterface xml_out (project_data, "OpenGeoSysGLI.xsd");

	std::string surface_fname (base_fname+".gml");
	xml_out.writeFile (surface_fname, gmsh_msh_fname);

	// get Surface for writing a OpenGeoSys TIN
	std::vector<GEOLIB::Surface*> const& sfcs(*(geo->getSurfaceVec(gmsh_msh_fname)));
	GEOLIB::Surface const*const sfc(sfcs[0]);
	std::ofstream out((base_fname+".tin").c_str());
	for (size_t k(0); k<sfc->getNTriangles(); k++) {
		GEOLIB::Triangle const& tri_k(*((*sfc)[k]));
		for (size_t j(0); j<3; j++) {
			out << *(tri_k.getPoint(j)) << " " << std::flush;
		}
		out << std::endl;
	}
	out.close();

	delete project_data;
}
