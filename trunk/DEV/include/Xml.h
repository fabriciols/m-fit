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
		Project project;

		char	text[100];
		char	type[50];
		char	posTransition[50];
		char	posUserTransition[50];
		char	userCutThreshold[50];
		int		sizeNodes;
		int		itemNumber;

		QFile *file;

		int openXml(char *);
		int readXml(char *);
		int createXml(char *, Project *project);
		int closeXml();

		char *getText();
		char *getType();
		char *getPosTransition();
		char *getPosUserTransition();
		char *getUserCutThreshold();
		int getSizeNodes();
		void setItemNumber(int);
};
#endif
