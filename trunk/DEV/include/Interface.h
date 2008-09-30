#include <QObject>
#include "QMainWindow"
#include "cv.h"
#include "highgui.h"

#include "../ui_mfit.h" 

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"

#ifndef INTERFACE_INCLUDE
#define INTERFACE_INCLUDE
class mfit: public QMainWindow
{
	Q_OBJECT

	public:
		mfit(QMainWindow *parent = 0);

		void changeWindowTitle(char *string);
		void insertVideoProperty(char *param_cy, char *value_cy);
		void clearVideoProperty();
		void updateVideoPlayer(Frame *frame);

		Ui::MainWindow ui;

	private slots:
		void on_playButton_clicked();
		void on_actionOpenProject_triggered();
		void on_actionSaveAs_triggered();
		void on_actionLoadVideo_triggered();

		void on_pauseButton_clicked();
		void on_stopButton_clicked();

		void updateVideoPlayer(QImage *image);
		
};
#endif
