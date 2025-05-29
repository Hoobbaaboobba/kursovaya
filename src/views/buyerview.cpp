#include "buyerview.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QChart>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QMimeData>
#include <QDrag>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QApplication>

BuyerView::BuyerView(QWidget *parent)
    : QTableView(parent)
    , contextMenu(new QMenu(this))
    , toolBar(new QToolBar(this))
    , proxyModel(new QSortFilterProxyModel(this))
    , buyerModel(nullptr)
    , currentSortColumn(-1)
    , currentSortOrder(Qt::AscendingOrder)
{
    // Configure view for drag and drop
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    // Disable double-click editing
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Setup header
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    // Setup context menu and toolbar
    setupContextMenu();
    setupToolBar();

    // Connect signals
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &BuyerView::onHeaderClicked);
    connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &BuyerView::onCustomContextMenu);
}

void BuyerView::setModel(BuyerModel *model)
{
    buyerModel = model;
    proxyModel->setSourceModel(model);
    QTableView::setModel(proxyModel);
}

void BuyerView::setupContextMenu()
{
    contextMenu->addAction(tr("Добавить покупателя"), this, &BuyerView::onAddBuyer);
    contextMenu->addAction(tr("Редактировать покупателя"), this, &BuyerView::onEditBuyer);
    contextMenu->addAction(tr("Удалить покупателя"), this, &BuyerView::onDeleteBuyer);
    contextMenu->addSeparator();
    contextMenu->addAction(tr("Поиск"), this, &BuyerView::onSearch);
    contextMenu->addAction(tr("Печать"), this, &BuyerView::onPrint);
    contextMenu->addAction(tr("Показать диаграмму"), this, &BuyerView::onShowChart);
}

void BuyerView::setupToolBar()
{
    toolBar->setIconSize(QSize(24, 24));
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setStyleSheet("QToolBar { spacing: 5px; padding: 5px; }");

    // Open File
    QAction *openAction = toolBar->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Открыть"));
    connect(openAction, &QAction::triggered, this, &BuyerView::onOpen);

    // Save
    QAction *saveAction = toolBar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Сохранить"));
    connect(saveAction, &QAction::triggered, this, &BuyerView::onSave);

    toolBar->addSeparator();

    // Add Buyer
    QAction *addAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), tr("Добавить покупателя"));
    connect(addAction, &QAction::triggered, this, &BuyerView::onAddBuyer);

    // Edit Buyer
    QAction *editAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), tr("Редактировать покупателя"));
    connect(editAction, &QAction::triggered, this, &BuyerView::onEditBuyer);

    // Delete Buyer
    QAction *deleteAction = toolBar->addAction(style()->standardIcon(QStyle::SP_TrashIcon), tr("Удалить покупателя"));
    connect(deleteAction, &QAction::triggered, this, &BuyerView::onDeleteBuyer);

    toolBar->addSeparator();

    // Search
    QAction *searchAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogContentsView), tr("Поиск"));
    connect(searchAction, &QAction::triggered, this, &BuyerView::onSearch);

    // Print
    QAction *printAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogListView), tr("Печать"));
    connect(printAction, &QAction::triggered, this, &BuyerView::onPrint);

    // Show Chart
    QAction *chartAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), tr("Показать диаграмму"));
    connect(chartAction, &QAction::triggered, this, &BuyerView::onShowChart);

    // Add toolbar to the parent widget if it's a QMainWindow
    if (QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parentWidget())) {
        // Force toolbar to be on a new line
        mainWindow->addToolBarBreak(Qt::TopToolBarArea);
        mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setMovable(false);
    }
}

void BuyerView::setupDragAndDrop()
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void BuyerView::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->exec(event->globalPos());
}

void BuyerView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-buyer-data") ||
        event->mimeData()->hasHtml() ||
        event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

void BuyerView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-buyer-data") ||
        event->mimeData()->hasHtml() ||
        event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

void BuyerView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-buyer-data")) {
        QByteArray itemData = event->mimeData()->data("application/x-buyer-data");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QVector<int> sourceRows;
        dataStream >> sourceRows;

        QModelIndex targetIndex = indexAt(event->pos());
        if (targetIndex.isValid()) {
            int targetRow = targetIndex.row();
            emit rowsMoved(sourceRows, targetRow);
            event->acceptProposedAction();
        }
    }
}

void BuyerView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QTableView::mousePressEvent(event);
}

void BuyerView::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QModelIndex index = indexAt(dragStartPosition);
    if (!index.isValid())
        return;

    startDrag(Qt::MoveAction);
}

void BuyerView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectionModel()->selectedRows();
    if (indexes.isEmpty())
        return;

    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    // Store selected rows data
    QVector<int> selectedRows;
    for (const QModelIndex &index : indexes) {
        selectedRows.append(index.row());
    }
    dataStream << selectedRows;
    mimeData->setData("application/x-buyer-data", itemData);

    // Create HTML content for external applications
    QString htmlContent = "<table border='1'>";
    htmlContent += "<tr>";
    for (int i = 0; i < model()->columnCount(); ++i) {
        htmlContent += "<th>" + model()->headerData(i, Qt::Horizontal).toString() + "</th>";
    }
    htmlContent += "</tr>";

    for (int row : selectedRows) {
        htmlContent += "<tr>";
        for (int col = 0; col < model()->columnCount(); ++col) {
            QModelIndex index = model()->index(row, col);
            htmlContent += "<td>" + index.data().toString() + "</td>";
        }
        htmlContent += "</tr>";
    }
    htmlContent += "</table>";
    mimeData->setHtml(htmlContent);

    // Create plain text content
    QString plainText;
    for (int row : selectedRows) {
        QStringList rowData;
        for (int col = 0; col < model()->columnCount(); ++col) {
            QModelIndex index = model()->index(row, col);
            rowData << index.data().toString();
        }
        plainText += rowData.join("\t") + "\n";
    }
    mimeData->setText(plainText);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    
    // Start the drag operation
    drag->exec(supportedActions);
}

void BuyerView::onHeaderClicked(int column)
{
    if (currentSortColumn == column) {
        currentSortOrder = (currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        currentSortColumn = column;
        currentSortOrder = Qt::AscendingOrder;
    }

    emit sortRequested(column, currentSortOrder);
}

void BuyerView::onCustomContextMenu(const QPoint &point)
{
    QMenu menu(this);
    menu.addAction(tr("Поиск в этом столбце"), [this, point]() {
        int column = horizontalHeader()->logicalIndexAt(point);
        emit searchRequested(column);
    });
    menu.exec(horizontalHeader()->mapToGlobal(point));
}

void BuyerView::onAddBuyer()
{
    emit addBuyerRequested();
}

void BuyerView::onEditBuyer()
{
    QModelIndexList indexes = selectionModel()->selectedRows();
    if (!indexes.isEmpty()) {
        emit editBuyerRequested(indexes.first().row());
    }
}

void BuyerView::onDeleteBuyer()
{
    QModelIndexList indexes = selectionModel()->selectedRows();
    if (!indexes.isEmpty()) {
        emit deleteBuyerRequested(indexes.first().row());
    }
}

void BuyerView::onSearch()
{
    bool ok;
    QString searchText = QInputDialog::getText(this, tr("Поиск"),
                                             tr("Введите текст поиска:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !searchText.isEmpty()) {
        int column = currentSortColumn;
        if (column == -1) {
            QStringList columns;
            for (int i = 0; i < model()->columnCount(); ++i) {
                columns << model()->headerData(i, Qt::Horizontal).toString();
            }
            column = QInputDialog::getItem(this, tr("Выберите столбец"),
                                         tr("Поиск в столбце:"), columns, 0, false, &ok).toInt();
        }
        if (ok) {
            emit searchRequested(column);
        }
    }
}

void BuyerView::onPrint()
{
    emit printRequested();
}

void BuyerView::onShowChart()
{
    emit showChartRequested();
}

void BuyerView::onSave()
{
    emit saveRequested();
}

void BuyerView::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"), "",
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));
        
    if (!fileName.isEmpty()) {
        emit openRequested(fileName);
    }
} 