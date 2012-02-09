/**
 * \file LegacyVtkInterface.h
 * 05/04/2011 LB Initial implementation
 */

#ifndef LEGACYVTKINTERFACE_H
#define LEGACYVTKINTERFACE_H

#include <fstream>
#include <string>
#include <vector>

namespace MeshLib
{
class CFEMesh;
}
class CRFProcess;
class ProcessInfo;

/// @brief Writes a legacy ascii vtk file of a mesh.
// TODO decouple from COutput
class LegacyVtkInterface
{
public:
	LegacyVtkInterface(MeshLib::CFEMesh* mesh,
	                   std::vector<std::string> pointArrayNames,
	                   std::vector<std::string> cellArrayNames,
	                   std::vector<std::string> materialPropertyArrayNames,
	                   std::string meshTypeName,
	                   ProcessInfo* processInfo);
	virtual ~LegacyVtkInterface();

	void WriteDataVTK(int number, double simulation_time, std::string baseFilename) const;

protected:
	void WriteVTKHeader(std::fstream&, int, double) const;
	void WriteVTKPointData(std::fstream&) const;
	void WriteVTKCellData(std::fstream&) const;
	void WriteVTKDataArrays(std::fstream&) const;
	void WriteELEVelocity(std::fstream &vtk_file) const;
    
    void printScalarArray(std::string arrayName, std::fstream &vtk_file) const;

	// Copied from COutput
	CRFProcess* GetPCS_ELE(const std::string &var_name) const;

	MeshLib::CFEMesh* _mesh;
	std::string _processType;
	std::vector<std::string> _pointArrayNames;
	std::vector<std::string> _cellArrayNames;
	std::vector<std::string> _materialPropertyArrayNames;
	std::string _meshTypeName;
	ProcessInfo* _processInfo;
};

#endif // LEGACYVTKINTERFACE_H
