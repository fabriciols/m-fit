#include <QObject>
#include "QDialog"

#include "../ui_cutConfig.h"

#ifndef CUTCONFIG_INCLUDE
#define CUTCONFIG_INCLUDE 

#define DEFAULT_CUT_THRESHOLD  45
#define DEFAULT_FIRST_CANNY   100
#define DEFAULT_LAST_CANNY    200

class CutConfig: public QDialog
{
	
	Q_OBJECT

	public:
		CutConfig(QDialog *parent = 0);

		void on_buttonOk_clicked();

		Ui::Dialog ui;

};

#endif
