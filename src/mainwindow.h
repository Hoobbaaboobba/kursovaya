#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include "models/buyermodel.h"
#include "controllers/buyercontroller.h"
#include "views/buyerview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void addBuyer();
    void editBuyer();
    void deleteBuyer();
    void about();
    void changeLanguage();
    void searchBuyers();
    void printView();
    void showChart();
    void updateWindowTitle();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void loadSettings();
    void saveSettings();
    void setupConnections();
    void setupContextMenu();
    void setupDragAndDrop();

    Ui::MainWindow *ui;
    BuyerModel *model;
    BuyerController *controller;
    BuyerView *view;
    QTranslator translator;
    QString currentFile;
};

#endif // MAINWINDOW_H 