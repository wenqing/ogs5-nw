/**
 * \file ProcessView.h
 * 2010/12/13 KR Initial implementation
 */

#ifndef PROCESSVIEW_H
#define PROCESSVIEW_H

#include <QContextMenuEvent>
#include <QTreeView>

#include "FEMCondition.h"

class ConditionModel;

/**
 * \brief A view for FEM-Conditions (Initial- & Boundary Conditions / Source Terms) with a number of additional
 * information such as Process Type, Distribution, etc.
 * \sa ConditionModel, CondItem
 */
class ProcessView : public QTreeView
{
	Q_OBJECT

public:
	/// Constructor
	ProcessView(QWidget* parent = 0);

	/// Update the view to visualise changes made to the underlying data
	void updateView();

protected slots:
	/// Instructions if the selection of items in the view has changed.
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
	/// Actions to be taken after a right mouse click is performed in the station view.
	void contextMenuEvent( QContextMenuEvent* e );

private slots:
	void on_Clicked(QModelIndex idx);
	void removeCondition();
	void removeProcess();
	void saveConditions();

signals:
	void conditionsRemoved(const FiniteElement::ProcessType, const std::string&, const FEMCondition::CondType);
	void itemSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
	void processRemoved(const FiniteElement::ProcessType);
	void saveConditionsRequested();
};

#endif //PROCESSVIEW_H
