#include "../include/QWidgetTimeline.h"
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
extern mfit* mfit_ui;

QWidgetTimeline::QWidgetTimeline(QWidget *parent) : QWidget(parent)
{ 
}

void QWidgetTimeline::mousePressEvent(QMouseEvent *event)
{
	QPoint point(this->pos());
	QPoint pointEvent(event->pos());
	QRect  rect(this->geometry());

	if (vdo_player->isRunning())
		return;

	if (currentProject->getVideo() == 0x0)
		return;

	Log::writeLog("%s :: widget x[%d] y[%d] press x[%d] y[%d]",
			__FUNCTION__,
			point.x(), point.y(),
			pointEvent.x(), pointEvent.y());

	if (pointEvent.x() >= 9 && pointEvent.x() <= rect.width() - 9)
	{
		Video *vdo;

		int posFrame;
		int x;

		// A widget tem 9 pixels antes de comecar realmente
		// a imagem da timeline
		x = pointEvent.x() - 9 ;

		vdo = currentProject->getVideo();

		//posFrame = cvRound(SIZE_SEC_FRAME*vdo->getFPS())*x / SIZE_FRAME_TIMELINE;
		posFrame = currentProject->TimelinePosToFrame(x);

		Log::writeLog("%s :: setting frame to pos [%d]", __FUNCTION__, posFrame);

		vdo->seekFrame(posFrame);

		Frame *frame;

		frame = vdo->getCurrentFrame();

		vdo_player->updatePlayer(frame);
		vdo_player->updateHist(frame);

		delete frame;

	}
	else
	{
		return;
	}

}

/*
	bool QWidgetTimeline::event(QEvent *e)
	{
	qDebug("received event(): %d", e->type());
	return QWidget::event(e);
	}
 */

void QWidgetTimeline::dropEvent(QDropEvent *event)
{
	Effect *effect = 0x0;
	Frame *frameNew = 0x0;
	Transition *transition = 0x0;
	Video *vdo = 0x0;
	unsigned int i = 0;
	long start = -1;
	long end = 0;

	switch(getItemByEvent(event))
	{
		case COLOR:
			effect = new Color();
			break;

		case ERODE:
			effect = new Erode();
			break;

		case DILATE:
			effect = new Dilate();
			break;

		default:
			break;

	}

	if (effect == 0x0)
		return;

	// Pego os pontos onde o efeito vai ser aplicado
	for (i = 0; i < currentProject->transitionList.size() ; i++)
	{
		transition = &currentProject->transitionList.at(i);

		if (start >= 0)
		{
			if (transition->selected == false)
			{
				end = transition->getPosCurrent();
				break;
			}
			else
			{
				continue;
			}
		}

		if (start == -1)
		{
			if (transition->selected == true)
			{
				start = transition->getPosCurrent();
				continue;
			}	
		}

	}

	if (end == 0x0)
	{
		vdo = currentProject->getVideo();
		end = (long)vdo->getFramesTotal();
	}

	effect->frameStart = start;
	effect->frameEnd   = end;

	currentProject->effectList.push_back(effect);

	mfit_ui->clearTransitionHeader();
	mfit_ui->updateEffectTree();

	if (vdo == 0x0)
		vdo = currentProject->getVideo();

	frameNew = vdo->getCurrentFrame();

	vdo_player->updatePlayer(frameNew);
	vdo_player->updateHist(frameNew);

}

void QWidgetTimeline::dragEnterEvent(QDragEnterEvent * event)
{
	int ret_i = 0;

	// Se nao tem timeline
	if (vdo_player->frameTimeline == 0)
		return;

	ret_i = selectDropTransition(event->pos());

	// Se modificamos a timeline, aceita o evento
	if (ret_i == 0)
		event->acceptProposedAction();

	return;

}

void QWidgetTimeline::dragMoveEvent(QDragMoveEvent *event)
{
	int ret_i = 0;

	// Se nao tem timeline
	if (vdo_player->frameTimeline == 0)
		return;

	// Se tem o CTRL pressionado, nao apagamos o anterior
	if (event->keyboardModifiers() & Qt::ControlModifier)
	{
		ret_i = selectDropTransition(event->pos(), 0);
	}
	else
	{
		ret_i = selectDropTransition(event->pos());
	}

	// Se modificamos a timeline, aceita o evento
	if (ret_i == 0)
		event->acceptProposedAction();

	return;
}

void QWidgetTimeline::dragLeaveEvent(QDragLeaveEvent *event)
{
	// Simpelsmente limpa o cabecalho da timeline
	mfit_ui->clearTransitionHeader();
	mfit_ui->updateTimeline();
}


int QWidgetTimeline::getItemByEvent(QDropEvent *event)
{
	int item = -1;

	if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
	{
		QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&itemData, QIODevice::ReadOnly);

		int r, c;
		QMap<int, QVariant> v;
		stream >> r >> c >> v;

		item = r;
	}

	return item;

}

int QWidgetTimeline::getTransitionByPos(QPoint pointEvent)
{
	int x;
	long pos;
	unsigned int i;
	// Por padrao deixamos o size, pois se nao achar nenhum
	// nao entra nas condicoes
	unsigned int transitionID = currentProject->transitionList.size();

	Transition* transition = 0x0;

	x = pointEvent.x() - 9 ;
	pos = currentProject->TimelinePosToFrame(x);

	// Temos que achar se tem alguma transicao no ponto indicado
	// Vamos de traz pra frente ate achar uma transicao
	// onde o ponto clicado eh maior que o ponto de inicio da transicao
	for (i = currentProject->transitionList.size()-1 ; (signed)i >= 0 ; i--)
	{
		transition = &currentProject->transitionList.at(i);

		if (pos >= transition->getPosCurrent())
		{
			// Achamos
			transitionID = i;
			break;
		}	
	}

	return transitionID;
}

int QWidgetTimeline::selectDropTransition(QPoint pointEvent, int clear)
{
	unsigned int transitionID;

	// Se nao tem timeline
	if (vdo_player->frameTimeline == 0)
		return 1;

	transitionID = getTransitionByPos(pointEvent);

	if (clear == 1)
	{
		mfit_ui->clearTransitionHeader();
	}

	if (transitionID < currentProject->transitionList.size())
	{
		mfit_ui->updateTransitionHeader(transitionID, 0);
		mfit_ui->updateTimeline();
		return 0;
	}

	return 1;
}
