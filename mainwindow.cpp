#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageLabel = new ClickableQLabel(this);
    setCentralWidget(imageLabel);
    this->open();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load image file"), QDir::currentPath());
    if(!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"), tr("Fail to load %1").arg(fileName));
            return;
        }

        // load image into label
        imageLabel->setPixmap(QPixmap::fromImage(image));
        // resize image
        imageLabel->resize(image.size());
    }
}
