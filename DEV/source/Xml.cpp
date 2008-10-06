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
xmlDocPtr Xml::openXml(char *xmlName)
{
	this->doc = xmlParseFile(xmlName);

	if(doc != NULL)
	{
		this->xmlName = xmlName;
		this->doc->children = doc->children;
		return (this->doc);
	}
	else
		return (NULL);
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
xmlDocPtr Xml::createXml(char *xmlName)
{
	this->doc = xmlNewDoc((const xmlChar*)"1.0");
	this->xmlName = xmlName;
	
	return (this->doc);
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
xmlNodePtr Xml::findNextTag(xmlNodePtr cur, int type, char *name, char *data)
{
	if(cur != NULL)
	{
		memcpy(name, 0, sizeof(name));
		memcpy(data, 0, sizeof(data));

		switch(type)
		{
			case 1:
				cur = cur->next;
			break;
			case 2:
				cur = cur->prev;
			break;
			case 3:
				cur = cur->parent;
			break;
			case 4:
				cur = cur->last;
			break;
		}
	
		if (cur->type == XML_ELEMENT_NODE || cur->type == XML_TEXT_NODE)		
		{
				strcpy(name, (char *)cur->name);
				strcpy(name, (char *)cur->content);
		}

		return (cur);
	}
	else
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
xmlNodePtr Xml::writeTag(char *name, char *data, int type, xmlDocPtr doc, xmlNodePtr tree)
{
	xmlNodePtr retTree;
	if(this->doc != NULL)
	{
		switch(type)
		{
			case 1:
				retTree = xmlNewDocNode(doc, NULL, (const xmlChar*)name, NULL);
			break;
			case 2:
				xmlSetProp(tree, (const xmlChar*)name, (const xmlChar*)data);
			break;
			case 3:
				retTree = xmlNewChild(tree, NULL, (const xmlChar*)name, (const xmlChar*)data);
			break;
		}
		return(retTree);
	}
	else
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
int Xml::closeXml(xmlDocPtr doc)
{
	if(this->doc != NULL)
	{
		xmlSaveFormatFile (this->xmlName, doc, 0);

		xmlFreeDoc(doc);
		xmlFreeDoc(this->doc);

		xmlCleanupParser();
		xmlMemoryDump();
		return (0);
	}
	else
		return(-1);
}
