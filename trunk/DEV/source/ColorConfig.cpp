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
* Construtor da janela de configura��es 
*************************************************************************
* param (E): QDialog -> referencia da janela que criou o objeto
************************************************************************
* Hist�rico
* 19/10/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
ColorConfig::ColorConfig(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

	// Se bot�o "Cancelar" for apertado, fecho a janela sem fazer nada.
	//connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));
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
