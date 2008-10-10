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
		this->file = &file;
		this->xmlName = xmlName;
		return (0);
	}
	else
	{
		file.close();
        QMessageBox::information(0, "Error", "Error opening file");
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
int Xml::findNextTag(char *tag)
{
/*
	int ret;
	QXmlSimpleReader xmlReader();

	ret = xmlReader.parse(&this->doc);

*/
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
        this->file->close();
		return (0);
}
