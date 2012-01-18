/**
 * \file QtTestBase.cpp
 * 2012-01-17 LB Implementation of QtTestBase class
 */

// ** INCLUDES **
#include "QtTestBase.h"

#include "Configure.h"

#include <QFile>
#include <QString>
#include <QTest>
#include <QTextStream>

#include "gdiff.h" // This should be the last include.


void QtTestBase::compareToReference(QString string, QString refFile)
{
	QString filePath(TESTDATAPATH);
	filePath += "/ref/";
	filePath += refFile;
	
	QFile qFile(filePath);
	if(!qFile.open(QIODevice::ReadOnly | QIODevice::Text))
		QFAIL(QString("Reference file %1 could not be read.").arg(refFile).toAscii());
		
	QString fileContent = qFile.readAll();
		
	// File compare
	diff_match_patch gdiff;
	QList<Diff> diffs = gdiff.diff_main(string, fileContent);
		
	if(diffs.length() > 0)
	{
		QString htmlOutput = gdiff.diff_prettyHtml(diffs);
		QString htmlOutputFilename(BUILDPATH);
		htmlOutputFilename += "/tests/results/";
		htmlOutputFilename += refFile;
		htmlOutputFilename += ".html";
		QFile htmlFile(htmlOutputFilename);
		if (!htmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
			QFAIL("Html output file could not be written.");
		QTextStream htmlStream(&htmlFile);
		htmlStream << htmlOutput;
		QFAIL(QString("Test link http://www.google.de or www.google.de File compare failed. See %1 for the differences.")
			.arg(htmlOutputFilename).toAscii());
	}
}

QString QtTestBase::getTestdataInputDir()
{
	QString dir(TESTDATAPATH);
	dir += "/input/";
	return dir;
}