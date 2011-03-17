/*
 * MeshQualityChecker.h
 *
 *  Created on: Dec 8, 2010
 *      Author: TF
 */

#ifndef MESHQUALITYCHECKER_H_
#define MESHQUALITYCHECKER_H_

#include <vector>

// MSH
#include "msh_mesh.h"

namespace Mesh_Group {

class MeshQualityChecker {
public:
	MeshQualityChecker(CFEMesh const * const mesh);

	virtual ~MeshQualityChecker () {};

	virtual void check () = 0;
	const std::vector<double>& getMeshQuality () const { return _mesh_quality_messure; }
	virtual void getHistogramm (std::vector<size_t>& histogramm) const;

protected:
	CFEMesh const * const _mesh;
	std::vector<double> _mesh_quality_messure;
	std::vector<size_t> _static_histogramm;
};

}

#endif /* MESHQUALITYCHECKER_H_ */
