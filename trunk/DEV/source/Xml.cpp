#include <QMessageBox>

#include "../include/Xml.h"
#include "../include///Log.h"

/************************************************************************
* Função que realiza o carregamento do arquivo xml. 
*************************************************************************
* param (E): char *xmlName => nome do xml.
************************************************************************
* Retorno: Sucesso
************************************************************************
* Histórico
* 04/10/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
int  Xml::openXml(char *xmlName)
{
	QFile file(xmlName);

	if (file.open(QIODevice::ReadWrite))
	{
		QDomDocument doc;
		QXmlSimpleReader xmlReader;
		QXmlInputSource *source = new QXmlInputSource(file);

		QString errmsg;	
		int errline, errcol;
	
		bool ok = this->doc.setContent(source, &xmlReader, &errmsg, &errline, &errcol);

		if (!ok)
		{
	        QMessageBox::information(0, "openXml", errmsg);
			return(1);
		}
	 
		this->xmlName = xmlName;

		file.close();
		return (0);
	}
	else
	{
		file.close();
        QMessageBox::information(0, "Erro", "Erro ao abrir arquivo XML");
		return(1);
	}
}

/************************************************************************
* Função que salva o documento xml. 
*************************************************************************
* param (E): nenhum.
************************************************************************
* Retorno: Sucesso
************************************************************************
* Histórico
* 04/10/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
int Xml::saveXml()
{
	QTextStream fileStream(this->file);
	this->doc.save(fileStream, 1);
	 
	return (0);
}

/************************************************************************
* Procura tag pelo XML
*************************************************************************
* param (E): xmlNodePtr cur => arvore XML
************************************************************************
* Retorno: Sucesso
************************************************************************
* Histórico
* 04/10/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
int Xml::readXml(char *tag, char *text, char *attr1, char *attr2, char *attr3, int *sizeNodes, int itemNumber)
{
	QDomNodeList nodeList;
	QByteArray txt;
	nodeList = this->doc.elementsByTagName(tag);

	*sizeNodes = nodeList.length();

	if(*sizeNodes > 0)
	{
		strcpy(text, nodeList.item(itemNumber).toElement().text());

		if(!strcmp(tag, "transition"))
		{
			strcpy(attr1, nodeList.item(itemNumber).toElement().attribute("type",NULL));
			strcpy(attr2, nodeList.item(itemNumber).toElement().attribute("posTransition",NULL));
			strcpy(attr3, nodeList.item(itemNumber).toElement().attribute("posUserTransition",NULL));
		}
		return(0);
	}
	else
	{
		QMessageBox::information(0, "tagname", "tag not found");
		return(1);
	}
}

/************************************************************************
* Escreve no arquivo XML aberto
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: Sucesso
************************************************************************
* Histórico
* 04/10/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
int Xml::writeTag(char *name, char *data)
{
	QDomText t = this->doc.createTextNode("Hello World");

	return(0);
}

/************************************************************************
* Fecha o arquivo XML
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: Sucesso
************************************************************************
* Histórico
* 04/10/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
int Xml::closeXml()
{
	delete &this->doc;
	return (0);
}
