/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *writeBlockCounter;
    QLabel *label_2;
    QLineEdit *readBlockCounter;
    QLabel *label_3;
    QLineEdit *ringBufferSize;
    QPushButton *start;
    QLineEdit *errorCounter;
    QLabel *label_4;
    QSpinBox *writeBlockSize;
    QLabel *label_5;
    QSpinBox *readBlockSize;
    QLabel *label_6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(354, 275);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        writeBlockCounter = new QLineEdit(centralwidget);
        writeBlockCounter->setObjectName("writeBlockCounter");

        formLayout->setWidget(0, QFormLayout::FieldRole, writeBlockCounter);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        readBlockCounter = new QLineEdit(centralwidget);
        readBlockCounter->setObjectName("readBlockCounter");

        formLayout->setWidget(2, QFormLayout::FieldRole, readBlockCounter);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_3);

        ringBufferSize = new QLineEdit(centralwidget);
        ringBufferSize->setObjectName("ringBufferSize");

        formLayout->setWidget(4, QFormLayout::FieldRole, ringBufferSize);

        start = new QPushButton(centralwidget);
        start->setObjectName("start");

        formLayout->setWidget(6, QFormLayout::FieldRole, start);

        errorCounter = new QLineEdit(centralwidget);
        errorCounter->setObjectName("errorCounter");

        formLayout->setWidget(5, QFormLayout::FieldRole, errorCounter);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        formLayout->setWidget(5, QFormLayout::LabelRole, label_4);

        writeBlockSize = new QSpinBox(centralwidget);
        writeBlockSize->setObjectName("writeBlockSize");

        formLayout->setWidget(1, QFormLayout::FieldRole, writeBlockSize);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);
        
        readBlockSize = new QSpinBox(centralwidget);
        readBlockSize->setObjectName("readChunkSize");
        
        formLayout->setWidget(3, QFormLayout::FieldRole, readBlockSize);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_6);


        horizontalLayout->addLayout(formLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 354, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Write block counter", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Read block counter", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Ringbuffer size", nullptr));
        start->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Errors", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Write block size", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Read block size", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
