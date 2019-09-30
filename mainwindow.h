#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImage>

#include <QMessageBox>
#include <QDebug>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_actionOpen_triggered();

	void on_cbFile_currentIndexChanged(int index);

	void on_dsbScale_valueChanged(double arg1);

	void on_cbLayer_currentIndexChanged(int index);

private:
	Ui::MainWindow *ui;
	QList<QImage> images;
	QStringList fileNames;
	int currentIndex;
	double currentScale, currentLayerScale;
	QLabel *lImage;

};

#endif // MAINWINDOW_H
