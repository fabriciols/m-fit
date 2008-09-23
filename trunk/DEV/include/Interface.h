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
		void on_actionOpenProject_triggered();
		void on_actionSaveAs_triggered();


	private:
		Ui::MainWindow ui;
};
