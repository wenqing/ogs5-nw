/**
 * \file XmlGspInterface.h
 * 2011/11/23 KR as derived class from XMLInterface
 */

#ifndef XMLGSPINTERFACE_H
#define XMLGSPINTERFACE_H

#include "ProjectData.h"
#include "XMLInterface.h"

class QFile;
class QDomDocument;
class QDomNode;
class QDomElement;

/**
 * \brief Reads and writes project information to and from XML files.
 */
class XmlGspInterface : public XMLInterface
{
public:
	/**
	 * Constructor
	 * \param project Project data.
	 * \param schemaFile An XML schema file (*.xsd) that defines the structure of a valid data file.
	 */
	XmlGspInterface(ProjectData* project, const std::string &schemaFile);

	/// Reads an xml-file containing a GeoSys project.
	/// Project files currently cover only geo-, msh- and station-data. This will be expanded in the future.
	int readFile(const QString &fileName);

	/// Writes a GeoSys project file containing all data that is currently loaded.
	/// Project files currently cover only geo-, msh- and station-data. This will be expanded in the future.
	int writeFile(const QString &fileName, const QString &tmp = "") const;

private:
};

#endif // XMLGSPINTERFACE_H
