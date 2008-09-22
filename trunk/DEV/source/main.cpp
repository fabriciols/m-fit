#include <QApplication>

#include "../include/Interface.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mfit mfit_ui;
    mfit_ui.show();
    return app.exec();
}
