#ifndef XML_INCLUDE
#define XML_INCLUDE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
/**
 *Classe responsável por detectar
 *o dissolve.
 */
class Xml
{
	private:
		xmlDocPtr doc;
		xmlNodePtr tree, subtree, root;
		char *xmlName;

	public:
		xmlDocPtr openXml(char *);
		xmlDocPtr createXml(char *);
		xmlNodePtr findNextTag(xmlNodePtr, int, char *, char *);
		xmlNodePtr writeTag(char *, char *, int , xmlDocPtr, xmlNodePtr);
		int closeXml(xmlDocPtr);
};
#endif
