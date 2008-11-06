#include "../include/QTreeWidgetEffect.h"
#include "cv.h"
#include "highgui.h"

#include "../include/ColorConfig.h"
#include "../include/Transition.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Time.h"
#include "../include/Log.h"
#include "../include/Video.h"
#include "../include/Effect.h"
#include "../include/Project.h"
#include "../include/VideoPlayer.h"
#include "../include/MorphologyEffect.h"

extern Project* currentProject;
extern VideoPlayer* vdo_player;
extern Interface* Interface_ui;

QTreeWidgetEffect::QTreeWidgetEffect(QWidget *parent) : QTreeWidget(parent)
{ 
	deleteAct = new QAction(tr("&Remover Efeito"), this);
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteItem()));
}

void QTreeWidgetEffect::contextMenuEvent(QContextMenuEvent *event)
{
	QList<QTreeWidgetItem *> itens;
	itens = Interface_ui->ui.effectsTree->selectedItems();

	if (itens.size() == 0)
		return;

	QMenu menu(this);
	menu.addAction(deleteAct);
	menu.exec(event->globalPos());
}

void QTreeWidgetEffect::deleteItem(void)
{
	QList<QTreeWidgetItem *> itens;
	QTreeWidgetItem *item;
	char string_cy[10];
	int ind_i = 0x0;

	itens = Interface_ui->ui.effectsTree->selectedItems();

	if (itens.size() == 0)
		return;

	foreach(item, itens)
	{
		memset(string_cy, '\0', sizeof(string_cy));

		Interface::QStringToChar(item->text(0), string_cy);

		ind_i = atoi(string_cy);

		currentProject->removeEffect(ind_i);
	}

	// Atualiza o cursor
	vdo_player->updateCurrentFrame();

	// Pode salvar
	Interface_ui->enableSaveButton();

}
