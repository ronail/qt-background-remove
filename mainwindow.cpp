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
//    imageLabel = ui->imageLabel;
//    loadImageButton = ui->loadImageButton;
//    exportButton = ui->exportButton;
    connect(ui->loadImageButton, SIGNAL(clicked(bool)), this, SLOT(showLoadImageDialog()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(showExportFileDialog()));
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
    }
}

void MainWindow::showExportFileDialog()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Export file to..."), QDir::currentPath(), "PNG");
    if (!filepath.isEmpty()) {
        ui->imageLabel->getImage()->save(filepath);
    }
}


void MainWindow::setImage(QString filepath)
{
    QImage tempImage(filepath);
    if (tempImage.isNull()) {
        QMessageBox::information(this, tr("Image Viewer"), tr("Fail to load %1").arg(filepath));
        return;
    }
    ui->imageLabel->setImage(&tempImage);
    this->filename = filenameFromPath(filepath);
}

static QString filenameFromPath(QString filepath){
    int index = filepath.lastIndexOf(QRegExp("[\\/]"));
     return filepath.right(filepath.length() - index - 1);
}

