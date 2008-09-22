#include <QObject>
#include "QMainWindow"
#include "../ui_mfit.h" 


class mfit: public QMainWindow
{
	Q_OBJECT

	public:
		mfit(QMainWindow *parent = 0);

	private slots:
		void on_okButton_clicked();

	private:
		Ui::MainWindow ui;
};
