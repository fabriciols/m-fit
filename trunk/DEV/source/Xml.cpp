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
	QDomDocument doc("mydocument");
	QFile file(xmlName);

	if (!file.open(QIODevice::ReadOnly))
		return(1);

	if (doc.setContent(&file))
	{
		this->xmlName = xmlName;
		this->doc = doc;
		return (0);
	}
	else
	{
		file.close();
		return(1);
	}
}

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
int Xml::createXml(char *xmlName)
{
	QDomDocument doc(xmlName);

	QDomElement root = doc.createElement("MyML");
	doc.appendChild(root);

	QDomElement tag = doc.createElement("Greeting");
	root.appendChild(tag);

	QDomText t = doc.createTextNode("Hello World");
	tag.appendChild(t);

	QString xml = doc.toString();
	 
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
int Xml::findNextTag(char *tag)
{
/*
	int ret;
	QXmlSimpleReader xmlReader();

	ret = xmlReader.parse(&this->doc);

	return(ret);
*/
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
		return (0);
}
