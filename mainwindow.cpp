#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <QMessageBox>

static QString filenameFromPath(QString filepath);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tolerancySlider->setTracking(false);
    ui->toleranceLabel->setText(QString::number(ClickableQLabel::TOLERANCE));
    ui->tolerancySlider->setValue(ClickableQLabel::TOLERANCE);
    connect(ui->loadImageButton, SIGNAL(clicked(bool)), this, SLOT(showLoadImageDialog()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(showExportFileDialog()));
//    connect(ui->resetButton, SIGNAL(clicked()), ui->imageLabel, SLOT(resetImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    showLoadImageDialog();
}

void MainWindow::showLoadImageDialog()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Load image file"), QDir::currentPath());
    if(!filepath.isEmpty()) {
        this->setImage(filepath);
        this->ui->exportButton->setEnabled(true);
        this->ui->resetButton->setEnabled(true);
    }
}

void MainWindow::showExportFileDialog()
{
    QString saveFilepath = QFileDialog::getSaveFileName(this, tr("Export file to..."), QDir::currentPath(), "PNG");
    if (!saveFilepath.isEmpty()) {
        ui->imageLabel->getImage()->save(saveFilepath);
    }
}


void MainWindow::setImage(QString filepath)
{
    QImage *tempImage = new QImage(filepath);
    if (tempImage->isNull()) {
        QMessageBox::information(this, tr("Image Viewer"), tr("Fail to load %1").arg(filepath));
        return;
    }
    ui->imageLabel->setImage(tempImage);
//    this->filepath = filenameFromPath(filepath);
    this->filepath = filepath;
    this->image = *tempImage;
}

static QString filenameFromPath(QString filepath){
    int index = filepath.lastIndexOf(QRegExp("[\\/]"));
     return filepath.right(filepath.length() - index - 1);
}


void MainWindow::on_tolerancySlider_valueChanged(int value)
{
    ClickableQLabel::TOLERANCE = value;
    ui->toleranceLabel->setText(QString::number(value));
    if (ui->imageLabel->pixmap()) {
        // reload the original image
        ui->imageLabel->setImage(&(this->image));
        // remove background using new tolerance value
        ui->imageLabel->removeBackground();
    }
}

void MainWindow::on_resetButton_clicked()
{
    ui->imageLabel->reset();
//    ui->imageLabel->setImage(this->image);
}

void MainWindow::on_previewCheckBox_toggled(bool checked)
{
    ui->tolerancySlider->setTracking(checked);
}
