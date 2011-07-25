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
    ClickableQLabel *imageRightLabel;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ClickableQLabel *imageLabel;
    QPushButton *loadImageButton;
    QPushButton *exportButton;
    QString filepath;
    QImage image;
    void setImage(QString filepath);
public slots:
    void showExportFileDialog();
    void showLoadImageDialog();
private slots:
    void open();
    void on_tolerancySlider_valueChanged(int value);
    void on_resetButton_clicked();
    void on_previewCheckBox_toggled(bool checked);
    void on_tolerancySlider_sliderReleased();
};

#endif // MAINWINDOW_H
