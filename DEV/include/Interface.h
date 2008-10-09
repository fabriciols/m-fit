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
		void on_actionSaveAs_triggered();
		void on_actionLoadVideo_triggered();

		void on_playButton_clicked();
		void on_pauseButton_clicked();
		void on_stopButton_clicked();

		void on_videoTime_timeChanged(const QTime & time);

		void updateVideoPlayer(QImage *image);
		void updateVideoPlayer(QImage *image, QImage *imageHist);
		void updateVideoPlayer(QImage *image, int type);

		void on_actionAllTransitions_triggered();
		void on_actionOnlyCuts_triggered();
		void on_actionAllFades_triggered();
		void on_actionOnlyDissolve_triggered();

		int askUser();

	public:
		mfit(QMainWindow *parent = 0);

		void changeWindowTitle(char *string);
		void insertVideoProperty(char *param_cy, char *value_cy);
		void clearVideoProperty();
		
		void updatePlayer(QImage *image);
		void updateHist(QImage *image);
		
		void setVideoTime(double framePos, double fps);

		void createTimeline(void);
		void setTimeline(Frame *timeLine);
		void updateTimeline();

		void insertTransitionsTree(Transition* transition); // Insere um item na lista de transições
		void insertTransitionsTimeline(Transition* transition); // Marca a timeline com uma linha onde houver transição.

		void updateTransitions();
		void clearTransitionsTree();

		Ui::MainWindow ui;

};

#endif
