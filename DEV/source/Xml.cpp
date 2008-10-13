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
	char txt[255];
	nodeList = this->doc.elementsByTagName(tag);
	int subNodesSize=0, i=0;
	QString item;

	*sizeNodes = nodeList.length();

	memset(text,0,sizeof(attr1));
	memset(attr1,0,sizeof(attr1));
	memset(attr2,0,sizeof(attr1));
	memset(attr3,0,sizeof(attr1));

	if(*sizeNodes > 0)
	{
		strcpy(text, nodeList.item(itemNumber).toElement().text());

		if(!strcmp(tag, "transition"))
		{
			if(nodeList.item(itemNumber).hasChildNodes())
			{
				nodeList = nodeList.item(itemNumber).childNodes();
				subNodesSize = nodeList.length();

				for(i=0;i<subNodesSize;i++)
				{
					if(!strcmp("type",nodeList.item(i).nodeName()))
						strcpy(attr1, nodeList.item(i).toElement().text());
					else if(!strcmp("posTransition",nodeList.item(i).nodeName()))
						strcpy(attr2, nodeList.item(i).toElement().text());
					else if(!strcmp("posUserTransition",nodeList.item(i).nodeName()))
						strcpy(attr3, nodeList.item(i).toElement().text());
					else if(!strcmp("label",nodeList.item(i).nodeName()))
						strcpy(text, nodeList.item(i).toElement().text());
				}
			}
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
int Xml::createXml(char *projectName, char *videoPath, std::vector<Transition> *transitionListXml)
{
	int i;

	Transition *transition = new Transition();

	QDomText t = this->doc.createTextNode("Hello World");

//	transition = &transitionListXml.at(i);

//	for (i=0;i<transitionListXml.size();i++)
//	{
//		type_i = transition->getType();
//		posTransition_l = transition->getPosTransition();
//		posUserTransition_l = transition->getPosUserTransition();
//	}

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
