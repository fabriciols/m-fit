#include "ui_mainwindow.h"
#include <QImage>

#include "cv.h"
#include "effect.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{

	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);

	private slots:
		///A declaração dos métodos vem abaixo.
		void aplicaEfeito();
		void abrirImagem();
};

