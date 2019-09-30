#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	lImage = new QLabel();
	lImage->setBackgroundRole(QPalette::Base);
	lImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	lImage->setScaledContents(true);

	ui->scrollArea->setWidget(lImage);
	ui->scrollArea->setWidgetResizable(false);

	currentIndex = -1;
	currentScale = 2.0;
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool compareDiagonal(QString& s1, QString& s2) {
	QImage i1(s1), i2(s2);
	if(i1.isNull()) return true;
	if(i2.isNull()) return false;
	return (i1.width() * i1.height() < i2.width() * i2.height());
}

void MainWindow::on_actionOpen_triggered()
{
	QStringList files = QFileDialog::getOpenFileNames(
				this, tr("Select Images"),
				QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
				"*.jpg *.png");

	if(!files.isEmpty()){
		images.clear();
		fileNames.clear();
	}
	else{
		return;
	}

	std::sort(files.begin(), files.end(), compareDiagonal);

	for(auto file : files){
		QImage img(file);
		if(!img.isNull()){
			images << img;
			fileNames << file.mid(file.lastIndexOf('/') + 1);
		}
		else{
			QMessageBox::warning(
						this, "Not an image",
						file +
						" is not an image or being corrupted."
						" It will be ignored.");
		}
	}

	currentIndex = 0;
	ui->cbFile->addItems(fileNames);
	on_dsbScale_valueChanged(ui->dsbScale->value());
}

void MainWindow::on_cbFile_currentIndexChanged(int index)
{
	currentIndex = index;
	lImage->setPixmap(QPixmap::fromImage(images.at(index)));
	lImage->adjustSize();

	ui->lSize->setText("Size: " + QString::number(images.at(index).width()) + ";" + QString::number(images.at(index).height()));

	on_dsbScale_valueChanged(ui->dsbScale->value());
}

void MainWindow::on_dsbScale_valueChanged(double arg)
{
	if(currentIndex >= 0){
		QStringList list;
		currentScale = arg;

		int w = images.at(currentIndex).width();
		int h = images.at(currentIndex).height();
		int lowest = (w <= h) ? w : h;
		int max = log(lowest) / log(currentScale) + 1;

		for(int i = 0; i < max; ++i){
			list << QString::number(i);
		}

		ui->cbLayer->clear();
		ui->cbLayer->addItems(list);
	}
}

void MainWindow::on_cbLayer_currentIndexChanged(int index)
{
	if(currentIndex >= 0){
		if(index > 0){
			currentLayerScale = pow(currentScale, index);
			int width = images.at(currentIndex).width() / currentLayerScale;
			int height = images.at(currentIndex).height() / currentLayerScale;
			QImage newImage = images.at(currentIndex).scaled(width, height);

			lImage->setPixmap(QPixmap::fromImage(newImage));
			ui->lSize->setText("Size: " + QString::number(width) + ";" + QString::number(height));
		}
		else{
			lImage->setPixmap(QPixmap::fromImage(images.at(currentIndex)));
			ui->lSize->setText("Size: " + QString::number(images.at(currentIndex).width()) + ";" + QString::number(images.at(currentIndex).height()));
		}
	}
}
