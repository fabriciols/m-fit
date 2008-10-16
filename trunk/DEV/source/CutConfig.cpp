#include "../ui_cutConfig.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QtGui>

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/CutConfig.h"
#include "../include/Project.h"

extern Project *currentProject;

/************************************************************************
* Construtor da janela de configura��es 
*************************************************************************
* param (E): QDialog -> referencia da janela que criou o objeto
************************************************************************
* Hist�rico
* 14/10/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

CutConfig::CutConfig(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

	/**
	 * Verifico se existem valores definidos pelo usu�rio. Se sim, utilizo 
	 * estes valores como valor inicial, sen�o utilizo o valor padr�o.
	**/
	if (currentProject->getUserThreshold()) // Limiar de corte
		ui.spinPercentage->setValue(currentProject->getUserThreshold());

	if (currentProject->getUserFirstCanny()) // Limiar m�nimo do Canny
		ui.spinFirstCanny->setValue(currentProject->getUserFirstCanny());

	if (currentProject->getUserLastCanny()) // Limiar m�ximo do Canny
		ui.spinLastCanny->setValue(currentProject->getUserLastCanny());

}


void CutConfig::on_buttonOk_clicked()
{
	int userCutThreshold = ui.spinPercentage->value();
	int userFirstCanny = ui.spinFirstCanny->value();
	int userLastCanny = ui.spinLastCanny->value();

	if ( !userCutThreshold )
	{
		//Interface::alertUser();
	}
	else if ( userCutThreshold != DEFAULT_CUT_THRESHOLD )
		currentProject->setUserThreshold(userCutThreshold);

	if ( !userFirstCanny )
	{
		//Interface::alertUser();
	}

	else if ( userFirstCanny != DEFAULT_FIRST_CANNY )
		currentProject->setUserFirstCanny(userFirstCanny);

	if ( !userLastCanny )
	{
		//Interface::alertUser();
	}
	else if ( userLastCanny != DEFAULT_LAST_CANNY )
	{
		currentProject->setUserLastCanny(userLastCanny);
	}

}

