#include <QObject>
#include "QMainWindow"
#include "cv.h"
#include "highgui.h"

#include "../ui_mfit.h" 

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Transition.h"

#ifndef INTERFACE_INCLUDE
#define INTERFACE_INCLUDE
class mfit: public QMainWindow
{
	Q_OBJECT

	private slots:
		void on_actionOpenProject_triggered();
		void on_actionSave_triggered();
		void on_actionSaveAs_triggered();
		void on_actionLoadVideo_triggered();

		void on_playButton_clicked();
		void on_pauseButton_clicked();
		void on_stopButton_clicked();

		void on_videoTime_timeChanged(const QTime & time);

		//void updateVideoPlayer(QImage *image);
		//void updateVideoPlayer(QImage *image, QImage *imageHist);

		void updatePlayer(QImage *image);
		void updateHist(QImage *image);

		void on_actionAllTransitions_triggered();
		void on_actionOnlyCuts_triggered();
		void on_actionAllFades_triggered();
		void on_actionOnlyDissolve_triggered();
		
		void on_transitionsTree_itemDoubleClicked(QTreeWidgetItem * item, int column);
		void on_actionCut_Settings_triggered();

		int askDetection(); // Pergunta se o usu�rio quer detectar todas as transi��es ap�s carregar o v�deo.
		void alertUser(); // Mostra uma mensagem de alerta se o usu�rio tentar fazer qqr coisa antes de dar um load num v�deo
		void enableControls(); // Habilita todos os comandos (bot�es) ap�s carregar um v�deo.

		int askNewThreshold();

	public:
		mfit(QMainWindow *parent = 0);

		void askCutThreshold(int threshold);

		void changeWindowTitle(char *string);
		void insertVideoProperty(char *param_cy, char *value_cy);
		void clearVideoProperty();
		
		void setVideoTime(double framePos, double fps);
		void createTimeline(void);
		void setTimeline(Frame *timeLine);
		void updateTimeline();

		void insertTransitionsTree(Transition* transition); // Insere um item na lista de transi��es
		void insertTransitionsTimeline(Transition* transition); // Marca a timeline com uma linha onde houver transi��o.

		void updateTransitions();
		void clearTransitionsTree();

		static char* QStringToChar(QString string, char* string_cy);

		Ui::MainWindow ui;

};

#endif
