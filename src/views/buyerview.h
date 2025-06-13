#ifndef BUYERVIEW_H
#define BUYERVIEW_H

#include <QTableView>
#include <QMenu>
#include <QToolBar>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QTabWidget>
#include "../models/buyermodel.h"
#include "../models/searchhighlightproxy.h"
#include <QMap>

class BuyerView : public QTabWidget
{
    Q_OBJECT

public:
    explicit BuyerView(QWidget *parent = nullptr);
    ~BuyerView();

    void setupContextMenu();
    void setupToolBar();
    void setupDragAndDrop();
    void addFile(const QString &fileName);
    void switchToFile(const QString &fileName);
    void closeCurrentTab();
    QModelIndexList getSelectedRows() const;
    int getCurrentSortColumn() const;
    QSortFilterProxyModel* getProxyModel();
    QTableView* currentView() const { return m_currentView; }
    void retranslateUi();

    // Public accessors for private members
    QString getCurrentFile() const { return currentFile; }
    BuyerModel* getModel(const QString &file) const { return models.value(file); }
    SearchHighlightProxy* getProxy(const QString &file) const { return proxies.value(file); }

public slots:
    void onSortOrderChanged(int column, Qt::SortOrder order);
    void onSearch();
    void onSearchTextChanged(const QString &text);
    void onDeleteBuyer();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void startDrag(Qt::DropActions supportedActions);

signals:
    void addBuyerRequested();
    void editBuyerRequested(int row);
    void deleteBuyerRequested(int row);
    void searchRequested(int column);
    void sortRequested(int column, Qt::SortOrder order);
    void printRequested();
    void showChartRequested();
    void saveRequested();
    void openRequested(const QString &fileName);
    void rowsMoved(const QVector<int>& sourceRows, int targetRow);
    void fileSwitched(const QString &fileName);

private slots:
    void onHeaderClicked(int column);
    void onCustomContextMenu(const QPoint &point);
    void onAddBuyer();
    void onEditBuyer();
    void onPrint();
    void onShowChart();
    void onSave();
    void onOpen();
    void onTabChanged(int index);
    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

private:
    void setupView(QTableView *view);
    void setupConnections(QTableView *view);
    void setupContextMenu(QTableView *view);
    void setupDragAndDrop(QTableView *view);

    QMenu *contextMenu;
    QToolBar *toolBar;
    int currentSortColumn;
    Qt::SortOrder currentSortOrder;
    QPoint dragStartPosition;
    QStringList openFiles;
    QString currentFile;
    QTableView *m_currentView;
    // Per-tab data structures:
    QMap<QString, BuyerModel*> models;
    QMap<QString, SearchHighlightProxy*> proxies;
    QMap<QString, QTableView*> views;
    QMap<int, QString> tabPaths;  // Store tab index to absolute path mapping
};

#endif // BUYERVIEW_H 