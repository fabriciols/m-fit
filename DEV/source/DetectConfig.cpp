#include "../ui_detectConfig.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QtGui>

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/DetectConfig.h"
#include "../include/Project.h"

#include "../include/Log.h"

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

DetectConfig::DetectConfig(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.listDetectConfig->setCurrentRow(0);
	ui.detectProperties->setCurrentIndex(0);

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

	// Se bot�o "Cancelar" for apertado, fecho a janela sem fazer nada.
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));

	// Se alterar o item da lista (menu com os tipos de transi��es), mudo a pagina de configura��es.
	connect(ui.listDetectConfig, SIGNAL(itemClicked(QListWidgetItem *)),
 			  this, SLOT(changePage())
			 );

}

/************************************************************************
* Salva as configura��es e fecha a janela.
*************************************************************************
* param (E): N�o h�.� q como vai cair de quinta... pra n�o parar 3 dias 
*************************************************************************
* return: nenhum.
*************************************************************************
* Hist�rico
* 18/10/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

void DetectConfig::on_okButton_clicked()
{
	int userCutThreshold = ui.spinPercentage->value();
	int userFirstCanny = ui.spinFirstCanny->value();
	int userLastCanny = ui.spinLastCanny->value();

	if ( userCutThreshold != DEFAULT_CUT_THRESHOLD )
		currentProject->setUserThreshold(userCutThreshold);

	if ( userFirstCanny != DEFAULT_FIRST_CANNY )
		currentProject->setUserFirstCanny(userFirstCanny);

	if ( userLastCanny != DEFAULT_LAST_CANNY )
		currentProject->setUserLastCanny(userLastCanny);

	QDialog::close();
}

/************************************************************************
* Salva as configura��es e fecha a janela.
*************************************************************************
* param (E): N�o h�.
*************************************************************************
* return: nenhum.
*************************************************************************
* Hist�rico
* 18/10/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

void DetectConfig::changePage()
{

	ui.detectProperties->setCurrentIndex(ui.listDetectConfig->currentRow());

}
