#include <QObject>
#include "QDialog"

#include "../ui_detectConfig.h"

#ifndef CUTCONFIG_INCLUDE
#define CUTCONFIG_INCLUDE 

#define DEFAULT_CUT_THRESHOLD  45
#define DEFAULT_FIRST_CANNY   100
#define DEFAULT_LAST_CANNY    200

class DetectConfig: public QDialog
{
	
	Q_OBJECT

	private slots:

		void on_okButton_clicked();
		void changePage();

	public:
		DetectConfig(QDialog *parent = 0);

		Ui::Dialog ui;

};

#endif