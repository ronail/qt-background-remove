/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Jul 20 15:56:25 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <clickableqlabel.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    ClickableQLabel *imageLabel;
    QPushButton *loadImageButton;
    QPushButton *exportButton;
    QPushButton *resetButton;
    QCheckBox *previewCheckBox;
    QLabel *toleranceLabel;
    QSlider *tolerancySlider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(459, 353);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 0, 401, 251));
        frame->setStyleSheet(QString::fromUtf8("background-image: url(:/background/checker.png);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        imageLabel = new ClickableQLabel(frame);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(0, 0, 401, 251));
        loadImageButton = new QPushButton(centralWidget);
        loadImageButton->setObjectName(QString::fromUtf8("loadImageButton"));
        loadImageButton->setGeometry(QRect(10, 260, 91, 32));
        exportButton = new QPushButton(centralWidget);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        exportButton->setEnabled(false);
        exportButton->setGeometry(QRect(110, 260, 91, 32));
        resetButton = new QPushButton(centralWidget);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        resetButton->setEnabled(false);
        resetButton->setGeometry(QRect(370, 260, 75, 31));
        previewCheckBox = new QCheckBox(centralWidget);
        previewCheckBox->setObjectName(QString::fromUtf8("previewCheckBox"));
        previewCheckBox->setGeometry(QRect(300, 260, 70, 31));
        toleranceLabel = new QLabel(centralWidget);
        toleranceLabel->setObjectName(QString::fromUtf8("toleranceLabel"));
        toleranceLabel->setGeometry(QRect(418, 234, 31, 20));
        toleranceLabel->setAlignment(Qt::AlignCenter);
        tolerancySlider = new QSlider(centralWidget);
        tolerancySlider->setObjectName(QString::fromUtf8("tolerancySlider"));
        tolerancySlider->setGeometry(QRect(420, 11, 31, 211));
        tolerancySlider->setMaximum(255);
        tolerancySlider->setOrientation(Qt::Vertical);
        tolerancySlider->setTickPosition(QSlider::NoTicks);
        MainWindow->setCentralWidget(centralWidget);
        frame->raise();
        loadImageButton->raise();
        exportButton->raise();
        resetButton->raise();
        previewCheckBox->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 459, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BackgroundRemover", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QString());
        loadImageButton->setText(QApplication::translate("MainWindow", "Load", 0, QApplication::UnicodeUTF8));
        exportButton->setText(QApplication::translate("MainWindow", "Export", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        previewCheckBox->setText(QApplication::translate("MainWindow", "Preview", 0, QApplication::UnicodeUTF8));
        toleranceLabel->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
