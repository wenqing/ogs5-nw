/**
 * \file ConvertGliToVtk.cpp
 * 06/05/2010 LB Initial implementation
 *
 * Implementation of ConvertGliToVtk utility
 */

// ** INCLUDES **
#include "GEOObjects.h"
#include "OGSIOVer4.h"
#include "Point.h"
#include "VtkPointsSource.h"

#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <vector>

#include <vtkPolyDataWriter.h>
#include <vtkSmartPointer.h>

using namespace boost::filesystem;
using namespace std;
using namespace GEOLIB;

// Replace file extension
void replaceExt(string& s, const string& newExt)
{
	string::size_type i = s.rfind('.', s.length());
	if (i != string::npos)
		s.replace(i + 1, newExt.length(), newExt);
}

// Converts gli to vtk. (atm only points)
// No arguments: batch convert all gli files
// file argument: convert only the specified file
int main (int argc, char const* argv[])
{
	vector<string> filenames;
	if (argc == 2)
		filenames.push_back(string(argv[1]));

	if (filenames.empty())
	{
		const boost::regex e(".+\\.gli");
		directory_iterator end;
		for (directory_iterator it("./"); it != end; ++it)
		{
			string curFile = it->path().filename(); //.string();
			if (regex_match(curFile, e))
				filenames.push_back(curFile);
		}
	}

	vtkPolyDataWriter* writer = vtkPolyDataWriter::New();

	for (vector<string>::const_iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		string filename(*it);
		cout << "Opening file " << filename << " ... " << endl << flush;

		GEOLIB::GEOObjects* geo (new GEOLIB::GEOObjects);
		FileIO::readGLIFileV4(filename, geo);
		const std::vector< Point* >* pnts = geo->getPointVec(filename);
		if (pnts)
		{
			string vtkFilename = filename;
			replaceExt(vtkFilename, "vtk");

			VtkPointsSource* vtkPoints = VtkPointsSource::New();
			vtkPoints->setPoints(pnts);
			writer->SetInputConnection(vtkPoints->GetOutputPort());
			writer->SetFileName(vtkFilename.c_str());
			writer->Write();
			vtkPoints->Delete();
		}

		// TODO convert polylines and surfaces as well

		delete geo;
	}

	writer->Delete(); // TODO crashes

	cout << "File conversion finished" << endl;

	return 0;
}
