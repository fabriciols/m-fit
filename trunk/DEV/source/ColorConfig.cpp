#include "../ui_colorConfig.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QtGui>

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Color.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/ColorConfig.h"
#include "../include/Project.h"

#include "../include/Log.h"

extern Project *currentProject;

/************************************************************************
* Construtor da janela de configurações 
*************************************************************************
* param (E): QDialog -> referencia da janela que criou o objeto
************************************************************************
* Histórico
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
ColorConfig::ColorConfig(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

	// Se botão "Cancelar" for apertado, fecho a janela sem fazer nada.
	//connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));
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

/*
void ColorConfig::on_okButton_clicked()
{
	QDialog::close();
}
*/

void ColorConfig::on_rSlider_valueChanged(int value)
{

	char value_cy[5];
	sprintf(value_cy, "%d", value);
	QString string(value_cy);

	ui.rLabel->setText(string);

	r = value;

	return;
}

void ColorConfig::on_gSlider_valueChanged(int value)
{
	char value_cy[5];
	sprintf(value_cy, "%d", value);
	QString string(value_cy);

	ui.gLabel->setText(string);

	g = value;

	return;
}

void ColorConfig::on_bSlider_valueChanged(int value)
{
	char value_cy[5];
	sprintf(value_cy, "%d", value);
	QString string(value_cy);

	ui.gLabel->setText(string);

	b = value;

	return;
}

void ColorConfig::updatePreview()
{
	return;
}
