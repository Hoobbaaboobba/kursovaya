/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionExit;
    QAction *actionAddBuyer;
    QAction *actionEditBuyer;
    QAction *actionDeleteBuyer;
    QAction *actionSearch;
    QAction *actionPrint;
    QAction *actionShowChart;
    QAction *actionEnglish;
    QAction *actionRussian;
    QAction *actionAbout;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuLanguage;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 600);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName("actionSaveAs");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionAddBuyer = new QAction(MainWindow);
        actionAddBuyer->setObjectName("actionAddBuyer");
        actionEditBuyer = new QAction(MainWindow);
        actionEditBuyer->setObjectName("actionEditBuyer");
        actionDeleteBuyer = new QAction(MainWindow);
        actionDeleteBuyer->setObjectName("actionDeleteBuyer");
        actionSearch = new QAction(MainWindow);
        actionSearch->setObjectName("actionSearch");
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName("actionPrint");
        actionShowChart = new QAction(MainWindow);
        actionShowChart->setObjectName("actionShowChart");
        actionEnglish = new QAction(MainWindow);
        actionEnglish->setObjectName("actionEnglish");
        actionRussian = new QAction(MainWindow);
        actionRussian->setObjectName("actionRussian");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuLanguage = new QMenu(menubar);
        menuLanguage->setObjectName("menuLanguage");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuLanguage->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionAddBuyer);
        menuEdit->addAction(actionEditBuyer);
        menuEdit->addAction(actionDeleteBuyer);
        menuView->addAction(actionSearch);
        menuView->addAction(actionPrint);
        menuView->addAction(actionShowChart);
        menuLanguage->addAction(actionEnglish);
        menuLanguage->addAction(actionRussian);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\237\320\236 \321\203\321\207\321\221\321\202\320\260 \320\277\320\276\320\272\321\203\320\277\320\260\321\202\320\265\320\273\320\265\320\271 \320\266\320\270\320\273\321\213\321\205 \320\277\320\276\320\274\320\265\321\211\320\265\320\275\320\270\320\271", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214...", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        actionSaveAs->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272...", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        actionAddBuyer->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\277\320\276\320\272\321\203\320\277\320\260\321\202\320\265\320\273\321\217", nullptr));
        actionEditBuyer->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\277\320\276\320\272\321\203\320\277\320\260\321\202\320\265\320\273\321\217", nullptr));
        actionDeleteBuyer->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\277\320\276\320\272\321\203\320\277\320\260\321\202\320\265\320\273\321\217", nullptr));
        actionSearch->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272...", nullptr));
        actionPrint->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214...", nullptr));
        actionShowChart->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\264\320\270\320\260\320\263\321\200\320\260\320\274\320\274\321\203", nullptr));
        actionEnglish->setText(QCoreApplication::translate("MainWindow", "\320\220\320\275\320\263\320\273\320\270\320\271\321\201\320\272\320\270\320\271", nullptr));
        actionRussian->setText(QCoreApplication::translate("MainWindow", "\320\240\321\203\321\201\321\201\320\272\320\270\320\271", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\260\320\262\320\272\320\260", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\264", nullptr));
        menuLanguage->setTitle(QCoreApplication::translate("MainWindow", "\320\257\320\267\321\213\320\272", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "mainToolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
