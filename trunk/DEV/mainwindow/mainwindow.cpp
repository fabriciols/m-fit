#include "mainwindow.h"

#define EFFECT_DILATE 0

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)

{
	setupUi(this);

	connect(pushButton, SIGNAL(clicked()), this, SLOT(aplicaEfeito()));
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(abrirImagem()));
}

///Aqui, vem a implementação dos métodos declarados no arquivo mainwindow.h

void MainWindow::aplicaEfeito()
{
	char str_cy[30];

	Effect effect(image->toGray());
	Image* imgEffect;

	switch (comboBox->currentIndex())
	{
		case EFFECT_DILATE:
		default:
			imgEffect = effect.dilate();
	}

	label_2->setPixmap(QPixmap::fromImage(*(imgEffect->getQTimage())));

	sprintf(str_cy,"index [%d]",comboBox->currentIndex());
	label->setText(str_cy);
}

void MainWindow::abrirImagem()
{
	image = new Image("teste.jpg");

	label->setText("Load teste.jpg");

   label_2->setBackgroundRole(QPalette::Base);
   label_2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
   label_2->setScaledContents(true);

	label_2->setPixmap(QPixmap::fromImage(*(image->getQTimage())));
	label_2->pixmap();
}
