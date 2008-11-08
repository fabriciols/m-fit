#include "../ui_transitionEdit.h"

#include "QWidget.h"

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Color.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/TransitionEdit.h"
#include "../include/Project.h"
#include "../include/VideoPlayer.h"

#include "../include/Log.h"

extern Project *currentProject;
extern VideoPlayer *vdo_player;

/************************************************************************
* Construtor da janela de configurações 
*************************************************************************
* param (E): QDialog -> referencia da janela que criou o objeto
************************************************************************
* Histórico
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
TransitionEdit::TransitionEdit(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

	this->cancel = false;
	this->del = false;
	this->cancel = false;
	this->pos_l = 0;
}

void TransitionEdit::setID(unsigned int id)
{
	Transition* transition;
	Video* vdo;
	long pos_l;
	char pos_cy[10];

	transition = &currentProject->transitionList.at(id);
	vdo = currentProject->getVideo();
	pos_l = (long)transition->getPosCurrent();

	sprintf(pos_cy, "%ld", pos_l);

	ui.labelTransitionName->setText(transition->getLabel());
	ui.labelFramePos->setText(pos_cy);

	vdo->seekFrame(pos_l);

	updatePreview();

}

void TransitionEdit::updatePreview()
{

	Frame *frameNew;
	QImage *image = 0x0;
	Video *vdo = currentProject->getVideo();
	long pos_l;
	char pos_cy[10];

	frameNew = vdo->getCurrentFrame();

	image = frameNew->IplImageToQImage(&vdo_player->imageData, &vdo_player->imgWidth, &vdo_player->imgHeight);

	QPixmap pix_image = QPixmap::fromImage(*image);

	// Permite ajute da imagem em relação ao tamanho da janela
	ui.labelPreview->setScaledContents(true);
	ui.labelPreview->setPixmap(pix_image); // Pinta a imagem

	pos_l = (long)vdo->getCurrentPosition();
	sprintf(pos_cy, "%ld", pos_l);

	ui.labelCurrentPos->setText(pos_cy);

	delete frameNew;
	delete image;
}

void TransitionEdit::on_changeButton_clicked()
{
	Video *vdo;
	long pos_l;
	char pos_cy[10];
	vdo = currentProject->getVideo();

	pos_l = (long)vdo->getCurrentPosition();
	sprintf(pos_cy, "%ld", pos_l);
	ui.labelFramePos->setText(pos_cy);

	this->pos_l =  pos_l;
	this->change = true;
}

void TransitionEdit::on_deleteButton_clicked()
{
	this->del = true;
	this->change = false;
}

void TransitionEdit::on_cancelButton_clicked()
{
	this->cancel = true;
	QDialog::close();
}

void TransitionEdit::on_backButton_clicked()
{
	Video *vdo;
	vdo = currentProject->getVideo();

	vdo->seekFrame((long)vdo->getCurrentPosition()-1);

	updatePreview();
}
void TransitionEdit::on_forwardButton_clicked()
{
	Video *vdo;
	vdo = currentProject->getVideo();

	vdo->seekFrame((long)vdo->getCurrentPosition()+1);

	updatePreview();
}

void TransitionEdit::on_okButton_clicked()
{
	this->cancel = false;
	QDialog::close();

}
