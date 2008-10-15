#include "../ui_cutConfig.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QtGui>

#include "../include/Interface.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/CutConfig.h"
#include "../include/Project.h"

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

	if (currentProject->getUserThreshold())
		ui.spinPercentage->setValue(currentProject->getUserThreshold());

	if (currentProject->getUserFirstCanny())
		ui.spinFirstCanny->setValue(currentProject->getUserFirstCanny());

	if (currentProject->getUserLastCanny())
		ui.spinLastCanny->setValue(currentProject->getUserLastCanny());

}
