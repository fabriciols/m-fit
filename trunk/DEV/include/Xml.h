#ifndef XML_INCLUDE
#define XML_INCLUDE

#define	NODE		1
#define	ATTR		2
#define	VALUE		3
#define	ROOTNAME	5

#include <QXmlSimpleReader>
#include <QIODevice>
#include <QXmlInputSource>
#include <QtXml>
#include <QXmlReader>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 *Classe responsável por detectar
 *o dissolve.
 */
class Xml
{
	private:
		QDomDocument doc;
		char *xmlName;

	public:
		QFile *file;

		int openXml(char *);
		int saveXml();
		int readXml(char *);
		int writeTag(char *, char *);
		int closeXml();
};
#endif
