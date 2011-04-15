/**
 * \file GeoObjectListItem.h
 * 2011/04/05 KR Initial implementation
 *
 */
#ifndef CONDOBJECTLISTITEM_H
#define CONDOBJECTLISTITEM_H


#include "FEMCondition.h"
#include "TreeItem.h"
#include <vtkPolyDataAlgorithm.h>
#include "VtkConditionSource.h"
#include <QModelIndex>

/**
 * \brief The CondObjectListItem is the TreeItem that contains the subtree for either initial conditions, 
 * boundary conditions source terms. This item also contains the vtk source-item for visualisation of this
 * information and the indices of the associated geometry-objects.
 * \sa TreeItem
 */
class CondObjectListItem : public TreeItem
{

public:
	/// Constructor for the TreeItem specifying FEM Conditions.
	CondObjectListItem(const QList<QVariant> &data, TreeItem *parent, FEMCondition::CondType type, const std::vector<GEOLIB::Point*> *points, const std::vector<GEOLIB::Polyline*> *lines, const std::vector<GEOLIB::Surface*> *surfaces)
		: TreeItem(data, parent), _vtkSource(VtkConditionSource::New()),  _type(type), 
		  _pointsIdx(new std::vector<size_t>), _linesIdx(new std::vector<size_t>), _surfacesIdx(new std::vector<size_t>), _use_domain(new bool(false))
	{
		QString display_name = parent->data(0).toString().append(" - ").append(QString::fromStdString(FEMCondition::condTypeToString(type)));
		static_cast<VtkConditionSource*>(_vtkSource)->setData( points, lines, surfaces, _pointsIdx, _linesIdx, _surfacesIdx, _use_domain);
		static_cast<VtkConditionSource*>(_vtkSource)->SetName( display_name );
	}

	~CondObjectListItem()
	{
		_vtkSource->Delete();
		delete _pointsIdx;
		delete _linesIdx;
		delete _surfacesIdx;
		delete _use_domain;
	}

	/// Adds the index of a geometry-object associated with a FEM condition (necessary for construction of VTK object).
	void addIndex(GEOLIB::GEOTYPE type, size_t idx)
	{
		if (type==GEOLIB::POINT) _pointsIdx->push_back(idx);
		if (type==GEOLIB::POLYLINE) _linesIdx->push_back(idx);
		if (type==GEOLIB::SURFACE) _surfacesIdx->push_back(idx);
		if (type==GEOLIB::GEODOMAIN) *_use_domain = true;
		_vtkSource->Modified();
	}

	/// Returns the type of geo-objects contained in the subtree of this item.
	FEMCondition::CondType getType() { return _type; };

	/// Returns the Vtk polydata source object
	vtkPolyDataAlgorithm* vtkSource() const { return _vtkSource; };

private:
	/// The Vtk data source object. This is the starting point for a Vtk data
	/// visualization pipeline.
	vtkPolyDataAlgorithm* _vtkSource;

	FEMCondition::CondType _type;
	std::vector<size_t> *_pointsIdx;
	std::vector<size_t> *_linesIdx;
	std::vector<size_t> *_surfacesIdx;
	bool* _use_domain;
};

#endif //CONDOBJECTLISTITEM_H