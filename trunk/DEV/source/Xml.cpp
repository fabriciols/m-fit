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
* Procura tag pelo XML
*************************************************************************
* param (E): tag => nome da tag
* param (S): text => conteudo da tag
* param (S): type => atributo da tag transition do XML
* param (S): posTransition => atributo da tag transition do XML
* param (S): posUserTransition => atributo da tag transition do XML
* param (S): userCutThreshold => atributo da tag transition do XML
* param (S): sizeNodes => tamanho dos filhos da tag transitionList
* param (S): itemNumber => indice do nó a ser lido
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
	nodeList = this->doc.elementsByTagName(tag);
	int subNodesSize=0, i=0;
	QString item;

//char *text, char *type, char *posTransition, char *posUserTransition, char *userCutThreshold, int *sizeNodes, int itemNumber

	sizeNodes = nodeList.length();

	memset(text,0,sizeof(text));
	memset(type,0,sizeof(type));
	memset(posTransition,0,sizeof(posTransition));
	memset(posUserTransition,0,sizeof(posUserTransition));

	if(sizeNodes > 0)
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
						strcpy(type, nodeList.item(i).toElement().text());
					else if(!strcmp("posTransition",nodeList.item(i).nodeName()))
						strcpy(posTransition, nodeList.item(i).toElement().text());
					else if(!strcmp("posUserTransition",nodeList.item(i).nodeName()))
						strcpy(posUserTransition, nodeList.item(i).toElement().text());
					else if(!strcmp("userCutThreshold",nodeList.item(i).nodeName()))
						strcpy(userCutThreshold, nodeList.item(i).toElement().text());
					else if(!strcmp("label",nodeList.item(i).nodeName()))
						strcpy(text, nodeList.item(i).toElement().text());
				}
			}
		}
		return(0);
	}
	else
	{
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
int Xml::createXml(char *xmlName, char *projectName, char *videoPath, std::vector<Transition> *transitionListXml)
{
	int i;
	char str[100];
	QFile file(xmlName);

	if (file.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
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

		if((int)transitionListXml->size() > 0)
		{
			tag = doc.createElement("transitionlist");
			root.appendChild(tag);

			for(i=0;i<(int)transitionListXml->size();i++)
			{			
				Transition *transition;
				transition = &transitionListXml->at(i);

				// Controle pra nao salvar a transicao dummy (Inicio de Video)
				if (transition->getType() == 0)
					continue;

				tag2 = doc.createElement("transition");
				tag.appendChild(tag2);

				sprintf(str, "%d", transition->getType());
				tag3 = doc.createElement("type");
				tag2.appendChild(tag3);
				tag3.appendChild(doc.createTextNode(str));

				if(transition->getPosTransition() > 0)
				{
					sprintf(str, "%d", (int)transition->getPosTransition());
					tag3 = doc.createElement("posTransition");
					tag2.appendChild(tag3);
					tag3.appendChild(doc.createTextNode(str));
				}

//				Vai entrar futuramente tem q criar na Transition.h
//				if(transition->userCutThreshold > 0 && transition->userCutThreshold < 100)
//				{
//					tag3 = doc.createElement("userCutThreshold");
//					tag2.appendChild(tag3);
//					tag3.appendChild(doc.createTextNode("20"));
//				}

				if(transition->getPosUserTransition() > 0)
				{
					sprintf(str, "%d", (int)transition->getPosUserTransition());
					tag3 = doc.createElement("posUserTransition");
					tag2.appendChild(tag3);
					tag3.appendChild(doc.createTextNode(str));
				}

				tag3 = doc.createElement("label");
				tag2.appendChild(tag3);
				tag3.appendChild(doc.createTextNode(transition->getLabel()));
			}
		}
/*
		if((int)this->effectList->size() > 0)
		{
			tag = doc.createElement("effectList");
			root.appendChild(tag);

			for(i=0;i<(int)this->effectList->size();i++)
			{			
				tag2 = doc.createElement("effect");
				tag.appendChild(tag2);

				sprintf(str, "%d", i);
				tag3 = doc.createElement("id");
				tag2.appendChild(tag3);
				tag3.appendChild(doc.createTextNode(str));

				sprintf(str, "%ld", this->effectList->at(i)->frameStart);
				tag3 = doc.createElement("frameStart");
				tag2.appendChild(tag3);
				tag3.appendChild(doc.createTextNode(str));

				sprintf(str, "%ld", this->effectList->at(i)->frameEnd);
				tag3 = doc.createElement("frameEnd");
				tag2.appendChild(tag3);
				tag3.appendChild(doc.createTextNode(str));

				tag3 = doc.createElement("name_cy");
				tag2.appendChild(tag3);
				tag3.appendChild(doc.createTextNode(this->effectList->at(i)->name_cy));
			}
		}
*/
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

char *Xml::getText()
{
	return text;
}

char *Xml::getType()
{
	return type;
}

char *Xml::getPosTransition()
{
	return posTransition;
}

char *Xml::getPosUserTransition()
{
	return posUserTransition;
}

char *Xml::getUserCutThreshold()
{
	return userCutThreshold;
}

int Xml::getSizeNodes()
{
	return sizeNodes;
}

void Xml::setItemNumber(int item)
{
	itemNumber = item;
}
