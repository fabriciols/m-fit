#ifndef XML_INCLUDE
#define XML_INCLUDE

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
		int openXml(char *);
		int createXml(char *);
		int findNextTag(char *);
		int writeTag(char *, char *);
		int closeXml();
};
#endif
