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
int Xml::readXml(char *tag)
{
	QDomNodeList nodeList;
	QDomNode node;

	nodeList = this->doc.elementsByTagName("name");
	node = nodeList.item(0);
	QMessageBox::information(0, "Information", node.nodeValue());

	return(0);
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
//        this->file->close();
		return (0);
}
