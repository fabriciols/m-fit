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
* Construtor da janela de configurações 
*************************************************************************
* param (E): QDialog -> referencia da janela que criou o objeto
************************************************************************
* Histórico
* 14/10/08 - Thiago Mizutani
* Criação.
************************************************************************/

DetectConfig::DetectConfig(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.listDetectConfig->setCurrentRow(0);
	ui.detectProperties->setCurrentIndex(0);

	/**
	 * Verifico se existem valores definidos pelo usuário. Se sim, utilizo 
	 * estes valores como valor inicial, senão utilizo o valor padrão.
	**/
	if (currentProject->getUserThreshold()) // Limiar de corte
		ui.spinPercentage->setValue(currentProject->getUserThreshold());

	if (currentProject->getUserFirstCanny()) // Limiar mínimo do Canny
		ui.spinFirstCanny->setValue(currentProject->getUserFirstCanny());

	if (currentProject->getUserLastCanny()) // Limiar máximo do Canny
		ui.spinLastCanny->setValue(currentProject->getUserLastCanny());

	// Se botão "Cancelar" for apertado, fecho a janela sem fazer nada.
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));

	// Se alterar o item da lista (menu com os tipos de transições), mudo a pagina de configurações.
	connect(ui.listDetectConfig, SIGNAL(itemClicked(QListWidgetItem *)),
 			  this, SLOT(changePage())
			 );

}

/************************************************************************
* Salva as configurações e fecha a janela.
*************************************************************************
* param (E): Não há.é q como vai cair de quinta... pra não parar 3 dias 
*************************************************************************
* return: nenhum.
*************************************************************************
* Histórico
* 18/10/08 - Thiago Mizutani
* Criação.
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
* Salva as configurações e fecha a janela.
*************************************************************************
* param (E): Não há.
*************************************************************************
* return: nenhum.
*************************************************************************
* Histórico
* 18/10/08 - Thiago Mizutani
* Criação.
************************************************************************/

void DetectConfig::changePage()
{

	ui.detectProperties->setCurrentIndex(ui.listDetectConfig->currentRow());

}
