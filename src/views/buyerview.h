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
#include "../models/buyermodel.h"

class BuyerView : public QTableView
{
    Q_OBJECT

public:
    explicit BuyerView(QWidget *parent = nullptr);
    void setModel(BuyerModel *model);
    void setupContextMenu();
    void setupToolBar();
    void setupDragAndDrop();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

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

private slots:
    void onHeaderClicked(int column);
    void onCustomContextMenu(const QPoint &point);
    void onAddBuyer();
    void onEditBuyer();
    void onDeleteBuyer();
    void onSearch();
    void onPrint();
    void onShowChart();
    void onSave();
    void onOpen();

private:
    QMenu *contextMenu;
    QToolBar *toolBar;
    QSortFilterProxyModel *proxyModel;
    BuyerModel *buyerModel;
    int currentSortColumn;
    Qt::SortOrder currentSortOrder;
    QPoint dragStartPosition;
};

#endif // BUYERVIEW_H 