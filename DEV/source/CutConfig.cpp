#include "../ui_cutConfig.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QtGui>

#include "../include/CutConfig.h"

extern Project *currentProject;

/************************************************************************
* Construtor da janela de configurações 
*************************************************************************
* param (E): QDialog -> referencia da janela que criou o objeto
************************************************************************
* Histórico
* 14/10/08 - Thiago Mizutani
* Criação.
************************************************************************/

CutConfig::CutConfig(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);
}
