#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageLabel = ui->imageLabel;
    loadImageButton = ui->loadImageButton;
    connect(loadImageButton, SIGNAL(clicked(bool)), imageLabel, SLOT(loadImageFromDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    imageLabel->loadImageFromDialog();
}
