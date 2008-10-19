#include <QObject>
#include "QDialog"

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Color.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Project.h"

#include "../include/Log.h"

#include "../ui_colorConfig.h"

#ifndef COLORCONFIG_INCLUDE
#define COLORCONFIG_INCLUDE 

#include "../include/ColorConfig.h"

class ColorConfig: public QDialog
{
	
	Q_OBJECT

	private slots:

		//void on_okButton_clicked();
		void on_rSlider_valueChanged(int value);
		void on_gSlider_valueChanged(int value);
		void on_bSlider_valueChanged(int value);

	public:
		ColorConfig(QDialog *parent = 0);
		void updatePreview();

		Color *effectColor;
		Frame *framePreview;

		int r;
		int g;
		int b;

		Ui::colorDialog ui;
};

#endif
