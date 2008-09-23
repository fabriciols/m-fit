#include <QApplication>
#include "cv.h"
#include "highgui.h"

#include "../include/Interface.h"

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Project.h"

// Variaveis globais da aplicacao
Project *currentProject = 0;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mfit mfit_ui;
    mfit_ui.show();
    return app.exec();
}
