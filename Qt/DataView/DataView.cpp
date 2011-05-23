/**
 * \file DataView.cpp
 * 24/9/2009 LB Initial implementation
 *
 * Implementation of DataView
 */

#include "DataView.h"
#include <QHeaderView>
#include "GridAdapter.h"
#include "MshEditDialog.h"
#include "MshModel.h"
#include "MshItem.h"
#include "OGSError.h"

#include "VtkMeshSource.h"

#include <QObject>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QSettings>

#include "MeshIO/OGSMeshIO.h"

DataView::DataView( QWidget* parent /*= 0*/ )
: QTreeView(parent)
{
	//resizeColumnsToContents();
	//resizeRowsToContents();
}

void DataView::updateView()
{
	setAlternatingRowColors(true);
	setColumnWidth(0,125);
	size_t nColumns = (this->model() != NULL) ? this->model()->columnCount() : 0;
	for (size_t i=1; i<nColumns; i++)
		resizeColumnToContents(i);
}


void DataView::addMeshAction()
{
	QSettings settings("UFZ", "OpenGeoSys-5");
	QString fileName = QFileDialog::getOpenFileName(this, "Select mesh file", settings.value("lastOpenedFileDirectory").toString(), "OpenGeosys mesh files (*.msh);;All files (* *.*)");
	if (!fileName.isEmpty())
	{
		std::string name = fileName.toStdString();
		MeshLib::CFEMesh* msh = FileIO::OGSMeshIO::loadMeshFromFile(name);
		if (msh) static_cast<MshModel*>(this->model())->addMesh(msh, name);
	}
}

void DataView::removeMesh()
{
	emit requestMeshRemoval(this->selectionModel()->currentIndex());
}

void DataView::removeAllMeshes()
{
	TreeItem* root = static_cast<MshModel*>(this->model())->getItem(QModelIndex());
	int nChildren = root->childCount()-1;
	for (int i=nChildren; i>=0; i--)
		emit requestMeshRemoval(this->model()->index(i, 0, QModelIndex()));
}


void DataView::contextMenuEvent( QContextMenuEvent* event )
{
	QModelIndex index = this->selectionModel()->currentIndex();
	MshItem* item = dynamic_cast<MshItem*>(static_cast<TreeItem*>(index.internalPointer()));

	if (item)
	{
		QMenu menu;
		QAction* editMeshAction   = menu.addAction("Edit mesh...");
		QAction* checkMeshAction  = menu.addAction("Check mesh quality...");
		QAction* saveMeshAction   = menu.addAction("Save mesh...");
		menu.addSeparator();
		QAction* removeMeshAction = menu.addAction("Remove mesh");
		connect(editMeshAction, SIGNAL(triggered()), this, SLOT(openMshEditDialog()));
		connect(checkMeshAction, SIGNAL(triggered()), this, SLOT(checkMeshQuality()));
		connect(saveMeshAction, SIGNAL(triggered()), this, SLOT(writeMeshToFile()));
		connect(removeMeshAction, SIGNAL(triggered()), this, SLOT(removeMesh()));
		menu.exec(event->globalPos());
	}
}

void DataView::openMshEditDialog()
{
	MshModel* model = static_cast<MshModel*>(this->model());
	QModelIndex index = this->selectionModel()->currentIndex();
	const MeshLib::CFEMesh* mesh = static_cast<MshModel*>(this->model())->getMesh(index)->getCFEMesh();

	MshEditDialog meshEdit(mesh);
	connect(&meshEdit, SIGNAL(mshEditFinished(MeshLib::CFEMesh*, std::string&)), model, SLOT(addMesh(MeshLib::CFEMesh*, std::string&)));
	meshEdit.exec();
}

int DataView::writeMeshToFile() const
{
	QModelIndex index = this->selectionModel()->currentIndex();
	const MeshLib::CFEMesh* mesh = static_cast<MshModel*>(this->model())->getMesh(index)->getCFEMesh();

	if (mesh)
	{
		QString mshName = QString::fromStdString(static_cast<MshModel*>(this->model())->getMesh(index)->getName());
		std::string fileName = QFileDialog::getSaveFileName(NULL, "Save mesh as", mshName, "GeoSys mesh file (*.msh)").toStdString();

		if (!fileName.empty())
		{
			std::ofstream out (fileName.c_str());
			if (out.is_open()) {
				FileIO::OGSMeshIO::write (mesh, out);
				out.close();
				return 1;
			}
			else
				std::cout << "MshTabWidget::saveMeshFile() - Could not create file..." << std::endl;
		}
		else OGSError::box("No file name entered.");
	}
	return 0;
}

void DataView::checkMeshQuality ()
{
	QModelIndex index = this->selectionModel()->currentIndex();
	MshItem* item = static_cast<MshItem*>(static_cast<MshModel*>(this->model())->getItem(index));
	emit qualityCheckRequested(item->vtkSource());
}


/*
void DataView::selectionChanged( const QItemSelection &selected, const QItemSelection &deselected )
{
	emit itemSelectionChanged(selected, deselected);
	return QTreeView::selectionChanged(selected, deselected);
}

void DataView::selectionChangedFromOutside( const QItemSelection &selected, const QItemSelection &deselected )
{
	QItemSelectionModel* selModel = this->selectionModel();

	Q_ASSERT(selModel);

	selModel->blockSignals(true);
	selModel->select(deselected, QItemSelectionModel::Deselect);
	selModel->select(selected, QItemSelectionModel::Select);
	selModel->blockSignals(false);

	Model* model = static_cast<Model*>(this->model());
	//model->setSelectionFromOutside(selected, deselected);

	return QTreeView::selectionChanged(selected, deselected);
}

void DataView::clearSelection()
{
	selectionModel()->clearSelection();
}
*/

