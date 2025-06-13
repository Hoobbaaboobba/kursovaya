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
    void onOpen();
    void onSave();
    void saveFileAs();
    void addBuyer();
    void onEditBuyer();
    void onDeleteBuyer();
    void onSearch();
    void onPrint();
    void onShowChart();
    void about();
    void changeLanguage(const QString &language);
    void updateWindowTitle();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void setupConnections();
    void setupContextMenu();
    void setupDragAndDrop();
    void loadSettings();
    void saveSettings();
    void saveFile();

    // Helper methods for tab management
    BuyerModel* currentTabModel() const;
    QSortFilterProxyModel* currentTabProxy() const;

    Ui::MainWindow *ui;
    BuyerModel *model;
    BuyerController *controller;
    BuyerView *view;
    QString currentFile;
    QTranslator translator;
};

#endif // MAINWINDOW_H 