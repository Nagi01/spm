/********************************************************************************
** Form generated from reading UI file 'qt_toolbox.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_TOOLBOX_H
#define UI_QT_TOOLBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_toolBoxClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qt_toolBoxClass)
    {
        if (Qt_toolBoxClass->objectName().isEmpty())
            Qt_toolBoxClass->setObjectName(QStringLiteral("Qt_toolBoxClass"));
        Qt_toolBoxClass->resize(600, 400);
        menuBar = new QMenuBar(Qt_toolBoxClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        Qt_toolBoxClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qt_toolBoxClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Qt_toolBoxClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Qt_toolBoxClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Qt_toolBoxClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Qt_toolBoxClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Qt_toolBoxClass->setStatusBar(statusBar);

        retranslateUi(Qt_toolBoxClass);

        QMetaObject::connectSlotsByName(Qt_toolBoxClass);
    } // setupUi

    void retranslateUi(QMainWindow *Qt_toolBoxClass)
    {
        Qt_toolBoxClass->setWindowTitle(QApplication::translate("Qt_toolBoxClass", "Qt_toolBox", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Qt_toolBoxClass: public Ui_Qt_toolBoxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_TOOLBOX_H
