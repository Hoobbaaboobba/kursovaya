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
#include <QFileInfo>
#include <QDebug>
#include <algorithm>
#include <QToolBar>
#include <QKeySequence>
#include <QItemSelectionModel>
#include <QItemSelection>

BuyerView::BuyerView(QWidget *parent)
    : QTabWidget(parent),
      contextMenu(nullptr),
      toolBar(new QToolBar(this)),
      currentSortColumn(-1),
      currentSortOrder(Qt::AscendingOrder),
      m_currentView(nullptr)
{
    setTabsClosable(true);
    setMovable(true);
    setAcceptDrops(true);

    // Setup toolbar
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setIconSize(QSize(24, 24));

    QAction *addAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), tr("Add"));
    addAction->setShortcut(QKeySequence::New);
    connect(addAction, &QAction::triggered, this, &BuyerView::onAddBuyer);

    QAction *editAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), tr("Edit"));
    editAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(editAction, &QAction::triggered, this, &BuyerView::onEditBuyer);

    QAction *deleteAction = toolBar->addAction(style()->standardIcon(QStyle::SP_TrashIcon), tr("Delete"));
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &BuyerView::onDeleteBuyer);

    toolBar->addSeparator();

    QAction *searchAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogContentsView), tr("Search"));
    connect(searchAction, &QAction::triggered, this, &BuyerView::onSearch);

    QAction *printAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogListView), tr("Print"));
    connect(printAction, &QAction::triggered, this, &BuyerView::onPrint);

    QAction *chartAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), tr("Chart"));
    connect(chartAction, &QAction::triggered, this, &BuyerView::onShowChart);

    toolBar->addSeparator();

    QAction *saveAction = toolBar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &BuyerView::onSave);

    QAction *openAction = toolBar->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Open"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &BuyerView::onOpen);

    // Add toolbar to the parent widget if it's a QMainWindow
    if (QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parentWidget())) {
        mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
    }

    connect(this, &QTabWidget::tabCloseRequested, [this](int index) {
        QString absolutePath = tabPaths.value(index);
        removeTab(index);
        tabPaths.remove(index);
        if (views.contains(absolutePath)) {
            QTableView* view = views.take(absolutePath);
            delete view;
        }
        if (models.contains(absolutePath)) {
            BuyerModel* model = models.take(absolutePath);
            delete model;
        }
        if (proxies.contains(absolutePath)) {
            QSortFilterProxyModel* proxy = proxies.take(absolutePath);
            delete proxy;
        }
        if (currentFile == absolutePath) {
            currentFile.clear();
            m_currentView = nullptr;
            if (count() > 0) {
                setCurrentIndex(0);
                currentFile = tabPaths.value(0);
                m_currentView = views.value(currentFile);
            }
        }
    });

    connect(this, &QTabWidget::currentChanged, this, &BuyerView::onTabChanged);
}

BuyerView::~BuyerView()
{
    for (auto model : models) {
        delete model;
    }
    for (auto proxy : proxies) {
        delete proxy;
    }
    for (auto view : views) {
        delete view;
    }
}

void BuyerView::setupView(QTableView *view)
{
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setDragEnabled(true);
    view->setAcceptDrops(true);
    view->setDropIndicatorShown(true);
    view->setDragDropMode(QAbstractItemView::InternalMove);
    view->setSortingEnabled(true);
    view->horizontalHeader()->setSectionsMovable(true);
    view->horizontalHeader()->setStretchLastSection(true);
    view->verticalHeader()->setVisible(false);
    // Disable direct editing of cells
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void BuyerView::setupConnections(QTableView *view)
{
    connect(view->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &BuyerView::onHeaderClicked);
    connect(view->horizontalHeader(), &QHeaderView::sortIndicatorChanged,
            this, &BuyerView::onSortIndicatorChanged);
}

void BuyerView::setupContextMenu(QTableView *view)
{
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view, &QTableView::customContextMenuRequested,
            this, &BuyerView::onCustomContextMenu);
}

void BuyerView::setupDragAndDrop(QTableView *view)
{
    view->setDragEnabled(true);
    view->setAcceptDrops(true);
    view->setDropIndicatorShown(true);
    view->setDragDropMode(QAbstractItemView::InternalMove);
}

void BuyerView::addFile(const QString &fileName)
{
    QString absPath = QFileInfo(fileName).absoluteFilePath();
    
    // Check if file is already open
    if (models.contains(absPath)) {
        // Switch to existing tab
        int tabIndex = -1;
        for (int i = 0; i < count(); ++i) {
            if (tabPaths.value(i) == absPath) {
                tabIndex = i;
                break;
            }
        }
        if (tabIndex != -1) {
            setCurrentIndex(tabIndex);
        }
        return;
    }

    // Create new model and proxy
    BuyerModel* model = new BuyerModel(this);
    SearchHighlightProxy* proxy = new SearchHighlightProxy(this);
    proxy->setSourceModel(model);
    
    // Create new view
    QTableView* view = new QTableView(this);
    view->setModel(proxy);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Disable all edit triggers
    
    // Store references
    models[absPath] = model;
    proxies[absPath] = proxy;
    views[absPath] = view;
    
    // Add tab
    int tabIndex = addTab(view, QFileInfo(fileName).fileName());
    tabPaths[tabIndex] = absPath;
    setCurrentIndex(tabIndex);
    
    // Load data
    if (!model->loadFromFile(fileName)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load file: %1").arg(fileName));
        closeCurrentTab();
        return;
    }
    
    currentFile = absPath;
    m_currentView = view;
}

void BuyerView::closeCurrentTab()
{
    if (currentFile.isEmpty()) return;

    // Find the tab index for current file
    int tabIndex = -1;
    for (int i = 0; i < count(); ++i) {
        if (tabPaths.value(i) == currentFile) {
            tabIndex = i;
            break;
        }
    }

    if (tabIndex != -1) {
        removeTab(tabIndex);
        if (views.contains(currentFile)) {
            QTableView* view = views.take(currentFile);
            delete view;
        }
        if (models.contains(currentFile)) {
            BuyerModel* model = models.take(currentFile);
            delete model;
        }
        if (proxies.contains(currentFile)) {
            SearchHighlightProxy* proxy = proxies.take(currentFile);
            delete proxy;
        }
        tabPaths.remove(tabIndex);
        
        // Update current file and view
        currentFile.clear();
        m_currentView = nullptr;
        
        // Switch to first tab if available
        if (count() > 0) {
            setCurrentIndex(0);
            currentFile = tabPaths.value(0);
            m_currentView = views.value(currentFile);
        }
    }
}

void BuyerView::switchToFile(const QString &fileName)
{
    QString absolutePath = QFileInfo(fileName).absoluteFilePath();
    if (views.contains(absolutePath)) {
        // Find the tab index for this file
        for (auto it = tabPaths.begin(); it != tabPaths.end(); ++it) {
            if (it.value() == absolutePath) {
                setCurrentIndex(it.key());
                currentFile = absolutePath;
                m_currentView = views.value(absolutePath);
                emit fileSwitched(absolutePath);
                break;
            }
        }
    }
}

QModelIndexList BuyerView::getSelectedRows() const
{
    if (!m_currentView) return QModelIndexList();
    return m_currentView->selectionModel()->selectedRows();
}

int BuyerView::getCurrentSortColumn() const
{
    return currentSortColumn;
}

QSortFilterProxyModel* BuyerView::getProxyModel()
{
    if (!m_currentView) return nullptr;
    return proxies.value(currentFile);
}

void BuyerView::onSortOrderChanged(int column, Qt::SortOrder order)
{
    if (!m_currentView) return;
    m_currentView->sortByColumn(column, order);
}

void BuyerView::onTabChanged(int index)
{
    if (index >= 0) {
        QString absolutePath = tabPaths.value(index);
        currentFile = absolutePath;
        m_currentView = views.value(absolutePath);
        emit fileSwitched(absolutePath);
    } else {
        currentFile.clear();
        m_currentView = nullptr;
    }
}

void BuyerView::onHeaderClicked(int column)
{
    if (!m_currentView) return;
    currentSortColumn = column;
    currentSortOrder = (currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    m_currentView->sortByColumn(column, currentSortOrder);
    emit sortRequested(column, currentSortOrder);
}

void BuyerView::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    currentSortColumn = logicalIndex;
    currentSortOrder = order;
}

void BuyerView::onCustomContextMenu(const QPoint &point)
{
    if (!m_currentView) return;

    QMenu menu(this);
    QAction *addAction = menu.addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), tr("Add Buyer"));
    QAction *editAction = menu.addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), tr("Edit Buyer"));
    QAction *deleteAction = menu.addAction(style()->standardIcon(QStyle::SP_TrashIcon), tr("Delete Buyer"));
    menu.addSeparator();
    QAction *searchAction = menu.addAction(style()->standardIcon(QStyle::SP_FileDialogContentsView), tr("Search"));
    QAction *printAction = menu.addAction(style()->standardIcon(QStyle::SP_FileDialogListView), tr("Print"));
    QAction *showChartAction = menu.addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), tr("Show Chart"));

    QAction *selectedAction = menu.exec(m_currentView->viewport()->mapToGlobal(point));
    if (selectedAction == addAction) {
        emit addBuyerRequested();
    } else if (selectedAction == editAction) {
        QModelIndexList indexes = m_currentView->selectionModel()->selectedRows();
        if (!indexes.isEmpty()) {
            emit editBuyerRequested(indexes.first().row());
}
    } else if (selectedAction == deleteAction) {
        QModelIndexList indexes = m_currentView->selectionModel()->selectedRows();
        if (!indexes.isEmpty()) {
            QSortFilterProxyModel* proxy = proxies.value(currentFile);
            if (!proxy) return;

            QVector<int> sourceRows;
            QStringList buyerNames;
            for (const QModelIndex &index : indexes) {
                QModelIndex sourceIndex = proxy->mapToSource(index);
                if (sourceIndex.isValid()) {
                    sourceRows.append(sourceIndex.row());
                    Buyer buyer = models.value(currentFile)->getBuyer(sourceIndex.row());
                    buyerNames << QString("%1 %2").arg(buyer.firstName, buyer.lastName);
                }
            }

            if (!sourceRows.isEmpty()) {
                std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());
                QString message;
                if (sourceRows.size() == 1) {
                    message = tr("Are you sure you want to delete the following buyer?\n\n%1")
                             .arg(buyerNames.first());
                } else {
                    message = tr("Are you sure you want to delete the following %1 buyers?\n\n%2")
                             .arg(sourceRows.size())
                             .arg(buyerNames.join("\n"));
                }

                QMessageBox msgBox(this);
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setWindowTitle(tr("Confirm Deletion"));
                msgBox.setText(message);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);

                if (msgBox.exec() == QMessageBox::Yes) {
                    for (int row : sourceRows) {
                        if (models.value(currentFile)->removeBuyer(row)) {
                            emit deleteBuyerRequested(row);
                        }
                    }
                }
            }
        }
    } else if (selectedAction == searchAction) {
        emit searchRequested(currentSortColumn);
    } else if (selectedAction == printAction) {
        emit printRequested();
    } else if (selectedAction == showChartAction) {
        emit showChartRequested();
    }
}

void BuyerView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_currentView) return;
    onCustomContextMenu(event->pos());
}

void BuyerView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->acceptProposedAction();
    }
}

void BuyerView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->acceptProposedAction();
    }
}

void BuyerView::dropEvent(QDropEvent *event)
{
    if (!m_currentView) return;

    QModelIndex targetIndex = m_currentView->indexAt(event->pos());
    if (!targetIndex.isValid()) return;

    QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&itemData, QIODevice::ReadOnly);

    QVector<int> sourceRows;
    while (!stream.atEnd()) {
        int row, col;
        QMap<int, QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;
        sourceRows.append(row);
    }

    if (!sourceRows.isEmpty()) {
        emit rowsMoved(sourceRows, targetIndex.row());
    }
}

void BuyerView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QTabWidget::mousePressEvent(event);
}

void BuyerView::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance()) return;

    if (!m_currentView) return;
    QModelIndexList indexes = m_currentView->selectionModel()->selectedRows();
    if (indexes.isEmpty()) return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream stream(&itemData, QIODevice::WriteOnly);

    for (const QModelIndex &index : indexes) {
        stream << index.row() << index.column() << m_currentView->model()->itemData(index);
    }

    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

void BuyerView::setupContextMenu()
{
    if (!m_currentView) return;
    setupContextMenu(m_currentView);
}

void BuyerView::setupDragAndDrop()
{
    if (!m_currentView) return;
    setupDragAndDrop(m_currentView);
}

void BuyerView::onAddBuyer()
{
    emit addBuyerRequested();
}

void BuyerView::onEditBuyer()
{
    if (!m_currentView) return;
    QModelIndexList indexes = m_currentView->selectionModel()->selectedRows();
    if (!indexes.isEmpty()) {
        emit editBuyerRequested(indexes.first().row());
    }
}

void BuyerView::onDeleteBuyer()
{
    if (!m_currentView || currentFile.isEmpty() || !models.contains(currentFile)) {
        QMessageBox::warning(this, tr("Warning"), tr("No file is currently open"));
        return;
    }
    
    QModelIndexList indexes = m_currentView->selectionModel()->selectedRows();
    if (indexes.isEmpty()) {
        return;  // Just return if no selection, without showing warning
    }

    QSortFilterProxyModel* proxy = proxies.value(currentFile);
    BuyerModel* model = models.value(currentFile);
    if (!proxy || !model) {
        QMessageBox::warning(this, tr("Error"), tr("Internal error: model not found"));
        return;
    }

    QVector<int> sourceRows;
    QStringList buyerNames;
    for (const QModelIndex &index : indexes) {
        QModelIndex sourceIndex = proxy->mapToSource(index);
        if (sourceIndex.isValid()) {
            sourceRows.append(sourceIndex.row());
            Buyer buyer = model->getBuyer(sourceIndex.row());
            buyerNames << QString("%1 %2").arg(buyer.firstName, buyer.lastName);
        }
    }

    if (!sourceRows.isEmpty()) {
        std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());
        QString message;
        if (sourceRows.size() == 1) {
            message = tr("Are you sure you want to delete the following buyer?\n\n%1")
                     .arg(buyerNames.first());
        } else {
            message = tr("Are you sure you want to delete the following %1 buyers?\n\n%2")
                     .arg(sourceRows.size())
                     .arg(buyerNames.join("\n"));
        }

        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("Confirm Deletion"));
        msgBox.setText(message);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes) {
            for (int row : sourceRows) {
                if (model->removeBuyer(row)) {
                    emit deleteBuyerRequested(row);
                }
            }
        }
    }
}

void BuyerView::onSearch()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search"),
                                             tr("Enter search text:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !text.isEmpty()) {
        onSearchTextChanged(text);
    }
}

void BuyerView::onSearchTextChanged(const QString &text)
{
    if (currentFile.isEmpty() || !proxies.contains(currentFile))
        return;
        
    SearchHighlightProxy* proxy = proxies[currentFile];
    proxy->setSearchText(text);
    
    // Clear current selection
    if (m_currentView) {
        m_currentView->clearSelection();
    }
    
    // If there's search text, select matching rows
    if (!text.isEmpty() && m_currentView) {
        QItemSelectionModel* selectionModel = m_currentView->selectionModel();
        QItemSelection selection;
        
        for (int row = 0; row < proxy->rowCount(); ++row) {
            QModelIndex sourceIndex = proxy->mapToSource(proxy->index(row, 0));
            BuyerModel* model = qobject_cast<BuyerModel*>(proxy->sourceModel());
            if (model && model->rowMatchesSearch(sourceIndex.row(), text)) {
                QModelIndex firstIndex = proxy->index(row, 0);
                QModelIndex lastIndex = proxy->index(row, proxy->columnCount() - 1);
                selection.select(firstIndex, lastIndex);
        }
        }
        
        selectionModel->select(selection, QItemSelectionModel::Select | QItemSelectionModel::Rows);
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

void BuyerView::onOpen()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        tr("Open Files"), "",
        tr("Text Files (*.txt);;All Files (*)"));
        
    for (const QString &fileName : fileNames) {
        if (!fileName.isEmpty()) {
            emit openRequested(fileName);
        }
    }
}

void BuyerView::onSave()
{
    emit saveRequested();
}

void BuyerView::retranslateUi()
{
    // Update toolbar actions
    for (QAction* action : toolBar->actions()) {
        if (action->text() == "Add") {
            action->setText(tr("Add"));
        } else if (action->text() == "Edit") {
            action->setText(tr("Edit"));
        } else if (action->text() == "Delete") {
            action->setText(tr("Delete"));
        } else if (action->text() == "Search") {
            action->setText(tr("Search"));
        } else if (action->text() == "Print") {
            action->setText(tr("Print"));
        } else if (action->text() == "Chart") {
            action->setText(tr("Chart"));
        } else if (action->text() == "Save") {
            action->setText(tr("Save"));
        } else if (action->text() == "Open") {
            action->setText(tr("Open"));
        }
    }

    // Update tab titles
    for (int i = 0; i < count(); ++i) {
        QString fileName = QFileInfo(tabPaths.value(i)).fileName();
        setTabText(i, fileName);
    }
} 