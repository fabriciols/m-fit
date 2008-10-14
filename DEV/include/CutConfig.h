#include <QObject>
#include "QDialog"

#include "../ui_cutConfig.h"

#ifndef CUTCONFIG_INCLUDE
#define CUTCONFIG_INCLUDE 

class CutConfig: public QDialog
{
	
	Q_OBJECT

	public:
		CutConfig(QDialog *parent = 0);
		Ui::Dialog ui;

};

#endif
