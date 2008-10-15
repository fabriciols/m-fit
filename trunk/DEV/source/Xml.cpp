#include <QMessageBox>

#include "../include/Transition.h"
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
int Xml::readXml(char *tag, char *text, char *attr1, char *attr2, char *attr3, char *attr4, int *sizeNodes, int itemNumber)
{
	QDomNodeList nodeList;
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
					else if(!strcmp("userCutThreshold",nodeList.item(i).nodeName()))
						strcpy(attr4, nodeList.item(i).toElement().text());
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
int Xml::createXml(char *xmlName, char *projectName, char *videoPath, std::vector<Transition>* transitionListXml)
{
	int i;
	QFile file(xmlName);

	if (file.open(QIODevice::ReadWrite))
	{
		QDomElement tag;
		QDomElement tag2;
		QDomElement tag3;
		QDomDocument doc("mfit");
		QDomElement root = doc.createElement("projeto");
		doc.appendChild(root);

		QString string(xmlName);

		tag = doc.createElement("name");
		root.appendChild(tag);
		tag.appendChild(doc.createTextNode(string.right(string.length() - string.lastIndexOf("/") - 1)));

		tag = doc.createElement("path");
		root.appendChild(tag);
		tag.appendChild(doc.createTextNode(string.left(string.lastIndexOf("/"))));

		tag = doc.createElement("video");
		root.appendChild(tag);
		tag.appendChild(doc.createTextNode(videoPath));

		tag = doc.createElement("transitionlist");
		root.appendChild(tag);

//		for(i=0.i<transitionListXml.size();i++)
//		{
			tag2 = doc.createElement("transition");
			tag.appendChild(tag2);

			tag3 = doc.createElement("type");
			tag2.appendChild(tag3);
			tag3.appendChild(doc.createTextNode("2"));

			tag3 = doc.createElement("posTransition");
			tag2.appendChild(tag3);
			tag3.appendChild(doc.createTextNode("20"));

			tag3 = doc.createElement("userCutThreshold");
			tag2.appendChild(tag3);
			tag3.appendChild(doc.createTextNode("20"));

			tag3 = doc.createElement("posUserTransition");
			tag2.appendChild(tag3);
			tag3.appendChild(doc.createTextNode("100"));

			tag3 = doc.createElement("label");
			tag2.appendChild(tag3);
			tag3.appendChild(doc.createTextNode("Fade-In"));
//		}

		QString xml = doc.toString();

		QTextStream xmlStream(&file);
		xmlStream.setCodec("UTF-8");
		xmlStream<<xml.toUtf8();

		file.close();
		return (0);
	}
	else
	{
		file.close();
        QMessageBox::information(0, "Erro", "Erro ao abrir arquivo XML");
		return(1);
	}
	
	Transition *transition = new Transition();

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
