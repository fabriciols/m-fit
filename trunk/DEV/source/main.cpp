#include <QApplication>
#include "cv.h"
#include "highgui.h"

#include "../include/Interface.h"

#include "../include/Transition.h"
#include "../include/Project.h"


// Variaveis globais da aplicacao
Project *currentProject;
mfit *mfit_ui;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	 mfit_ui = new mfit();
	 currentProject = new Project();

    mfit_ui->show();
    return app.exec();
}
