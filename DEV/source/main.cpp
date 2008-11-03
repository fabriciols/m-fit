#include <QApplication>
#include <QMainWindow>
#include <QThread>
#include "cv.h"
#include "highgui.h"

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Transition.h"
#include "../include/Project.h"
#include "../include/VideoPlayer.h"

// Variaveis globais da aplicacao
Project *currentProject;
Interface *Interface_ui;
VideoPlayer *vdo_player;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	 vdo_player = new VideoPlayer();

	 Interface_ui = new Interface();

	 currentProject = new Project();

    Interface_ui->show();
    return app.exec();
}
