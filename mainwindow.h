#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <clickableqlabel.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ClickableQLabel *imageLabel;
    QPushButton *loadImageButton;
    QPushButton *exportButton;
    QString filename;
    void setImage(QString filepath);

private slots:
    void open();
    void showExportFileDialog();
    void showLoadImageDialog();
};

#endif // MAINWINDOW_H
