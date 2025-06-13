#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "views/buyerdialog.h"
#include "dialogs/aboutdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QChart>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QInputDialog>
#include <QTextDocument>
#include <QTextStream>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new BuyerModel(this))
    , controller(new BuyerController(model, this))
    , view(new BuyerView(this))
{
    ui->setupUi(this);
    setCentralWidget(view);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    setupConnections();
    setupContextMenu();
    setupDragAndDrop();

    loadSettings();
    
    // Load saved language preference
    QSettings settings;
    QString savedLanguage = settings.value("language", "en").toString();
    changeLanguage(savedLanguage);
    
    updateWindowTitle();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::createActions()
{
    // File actions
    ui->actionOpen->setShortcut(QKeySequence::Open);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpen);

    ui->actionSave->setShortcut(QKeySequence::Save);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSave);

    ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);

    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    ui->actionAddBuyer->setShortcut(QKeySequence::New);
    connect(ui->actionAddBuyer, &QAction::triggered, this, &MainWindow::addBuyer);

    ui->actionEditBuyer->setShortcut(QKeySequence("Ctrl+E"));
    connect(ui->actionEditBuyer, &QAction::triggered, this, &MainWindow::onEditBuyer);

    ui->actionDeleteBuyer->setShortcut(QKeySequence::Delete);
    connect(ui->actionDeleteBuyer, &QAction::triggered, this, &MainWindow::onDeleteBuyer);

    // View actions
    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::onSearch);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::onPrint);
    connect(ui->actionShowChart, &QAction::triggered, this, &MainWindow::onShowChart);

    // Language actions
    connect(ui->actionEnglish, &QAction::triggered, [this]() { changeLanguage("en"); });
    connect(ui->actionRussian, &QAction::triggered, [this]() { changeLanguage("ru"); });

    // Help actions
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    // File menu
    ui->menuFile->addAction(ui->actionOpen);
    ui->menuFile->addAction(ui->actionSave);
    ui->menuFile->addAction(ui->actionSaveAs);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ui->actionExit);

    // Edit menu
    ui->menuEdit->addAction(ui->actionAddBuyer);
    ui->menuEdit->addAction(ui->actionEditBuyer);
    ui->menuEdit->addAction(ui->actionDeleteBuyer);

    // View menu
    ui->menuView->addAction(ui->actionSearch);
    ui->menuView->addAction(ui->actionPrint);
    ui->menuView->addAction(ui->actionShowChart);

    // Language menu
    ui->menuLanguage->addAction(ui->actionEnglish);
    ui->menuLanguage->addAction(ui->actionRussian);

    // Help menu
    ui->menuHelp->addAction(ui->actionAbout);
}

void MainWindow::createToolBars()
{
    // File toolbar
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addSeparator();

    // Edit toolbar
    ui->mainToolBar->addAction(ui->actionAddBuyer);
    ui->mainToolBar->addAction(ui->actionEditBuyer);
    ui->mainToolBar->addAction(ui->actionDeleteBuyer);
    ui->mainToolBar->addSeparator();

    // View toolbar
    ui->mainToolBar->addAction(ui->actionSearch);
    ui->mainToolBar->addAction(ui->actionPrint);
    ui->mainToolBar->addAction(ui->actionShowChart);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готов"));
}

void MainWindow::createDockWindows()
{
    // Add any dock windows here if needed
}

void MainWindow::setupConnections()
{
    connect(view, &BuyerView::addBuyerRequested, this, &MainWindow::addBuyer);
    connect(view, &BuyerView::editBuyerRequested, this, &MainWindow::onEditBuyer);
    connect(view, &BuyerView::deleteBuyerRequested, this, &MainWindow::onDeleteBuyer);
    connect(view, &BuyerView::searchRequested, this, &MainWindow::onSearch);
    connect(view, &BuyerView::printRequested, this, &MainWindow::onPrint);
    connect(view, &BuyerView::showChartRequested, this, &MainWindow::onShowChart);
    connect(view, &BuyerView::sortRequested, controller, &BuyerController::sortByColumn);

    connect(controller, &BuyerController::fileOpened, this, &MainWindow::updateWindowTitle);
    connect(controller, &BuyerController::fileSaved, this, &MainWindow::updateWindowTitle);
    connect(controller, &BuyerController::error, this, [this](const QString &message) {
        QMessageBox::warning(this, tr("Ошибка"), message);
    });
    connect(controller, &BuyerController::sortOrderChanged, view, &BuyerView::onSortOrderChanged);

    connect(model, &BuyerModel::dataChanged, this, [this]() {
        updateWindowTitle();
        if (view->currentView()) {
            view->currentView()->viewport()->update();
        }
    });
    connect(model, &BuyerModel::modelReset, this, [this]() {
        updateWindowTitle();
        if (view->currentView()) {
            view->currentView()->viewport()->update();
        }
    });
}

void MainWindow::setupContextMenu()
{
    view->setupContextMenu();
}

void MainWindow::setupDragAndDrop()
{
    view->setupDragAndDrop();
}

void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    currentFile = settings.value("currentFile").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("currentFile", currentFile);
}

void MainWindow::onOpen()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        tr("Открыть файлы"), "",
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    for (const QString &fileName : fileNames) {
    if (!fileName.isEmpty()) {
        if (controller->openFile(fileName)) {
                view->addFile(fileName);
            currentFile = fileName;
            updateWindowTitle();
                statusBar()->showMessage(tr("Файл загружен"), 2000);
        } else {
                QMessageBox::warning(this, tr("Ошибка"),
                    tr("Не удалось открыть файл %1").arg(fileName));
            }
        }
    }
}

void MainWindow::saveFile()
{
    if (currentFile.isEmpty()) {
        saveFileAs();
    } else {
        if (controller->saveFile(currentFile)) {
            statusBar()->showMessage(tr("Файл сохранен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось сохранить файл %1").arg(currentFile));
        }
    }
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить файл"), QString(),
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    if (!fileName.isEmpty()) {
        if (controller->saveFile(fileName)) {
            currentFile = fileName;
            updateWindowTitle();
            statusBar()->showMessage(tr("Файл сохранен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось сохранить файл %1").arg(fileName));
        }
    }
}

// Helper to get the current tab's BuyerModel
BuyerModel* MainWindow::currentTabModel() const {
    if (!view->currentView()) return nullptr;
    QString file = view->getCurrentFile();
    return view->getModel(file);
}

// Helper to get the current tab's proxy
QSortFilterProxyModel* MainWindow::currentTabProxy() const {
    if (!view->currentView()) return nullptr;
    QString file = view->getCurrentFile();
    return view->getProxy(file);
}

void MainWindow::addBuyer()
{
    BuyerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString lastName, firstName, middleName;
        QString passportNumber, phoneNumber, email;
        QString propertyType, propertyAddress;
        double propertyArea, propertyPrice;
        QDate purchaseDate;
        QString paymentStatus, contractNumber;

        dialog.getBuyerData(lastName, firstName, middleName,
                          passportNumber, phoneNumber, email,
                          propertyType, propertyAddress, propertyArea,
                          propertyPrice, purchaseDate, paymentStatus,
                          contractNumber);

        Buyer buyer;
        buyer.lastName = lastName;
        buyer.firstName = firstName;
        buyer.middleName = middleName;
        buyer.passportNumber = passportNumber;
        buyer.phoneNumber = phoneNumber;
        buyer.email = email;
        buyer.propertyType = propertyType;
        buyer.propertyAddress = propertyAddress;
        buyer.propertyArea = QString::number(propertyArea);
        buyer.propertyPrice = QString::number(propertyPrice);
        buyer.purchaseDate = purchaseDate.toString("yyyy-MM-dd");
        buyer.paymentStatus = paymentStatus;
        buyer.contractNumber = contractNumber;

        BuyerModel* tabModel = currentTabModel();
        if (tabModel && tabModel->addBuyer(buyer)) {
            statusBar()->showMessage(tr("Покупатель добавлен"), 2000);
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось добавить покупателя"));
        }
    }
}

void MainWindow::onEditBuyer()
{
    BuyerModel* tabModel = currentTabModel();
    if (!tabModel) return;
    QModelIndexList indexes = view->getSelectedRows();
    if (!indexes.isEmpty()) {
    int row = indexes.first().row();
        Buyer buyer = tabModel->getBuyer(row);

    BuyerDialog dialog(this);
        
        // Create temporary variables for double values and date
        double propertyArea = buyer.propertyArea.toDouble();
        double propertyPrice = buyer.propertyPrice.toDouble();
        QDate purchaseDate = QDate::fromString(buyer.purchaseDate, "yyyy-MM-dd");
        
        dialog.setBuyerData(buyer.lastName, buyer.firstName, buyer.middleName,
                          buyer.passportNumber, buyer.phoneNumber, buyer.email,
                          buyer.propertyType, buyer.propertyAddress, 
                          propertyArea, propertyPrice,
                          purchaseDate,
                          buyer.paymentStatus, buyer.contractNumber);

    if (dialog.exec() == QDialog::Accepted) {
            dialog.getBuyerData(buyer.lastName, buyer.firstName, buyer.middleName,
                              buyer.passportNumber, buyer.phoneNumber, buyer.email,
                              buyer.propertyType, buyer.propertyAddress,
                              propertyArea, propertyPrice,
                              purchaseDate,
                              buyer.paymentStatus, buyer.contractNumber);
            
            // Convert values back to strings
            buyer.propertyArea = QString::number(propertyArea);
            buyer.propertyPrice = QString::number(propertyPrice);
            buyer.purchaseDate = purchaseDate.toString("yyyy-MM-dd");

            if (tabModel->updateBuyer(row, buyer)) {
                statusBar()->showMessage(tr("Покупатель обновлен"), 2000);
        } else {
                QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось обновить данные покупателя"));
            }
        }
    }
}

void MainWindow::onDeleteBuyer()
{
    // Let BuyerView handle the confirmation dialog and deletion
    view->onDeleteBuyer();
}

void MainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::changeLanguage(const QString &language)
{
    // Remove old translator
    qApp->removeTranslator(&translator);

    // Load new translation
    QString translationPath = QCoreApplication::applicationDirPath() + "/translations/buyerdatabase_" + language + ".ts";
    qDebug() << "Loading translation from:" << translationPath;
    
    if (translator.load(translationPath)) {
            qApp->installTranslator(&translator);
        
        // Update UI
            ui->retranslateUi(this);
        
        // Update window title
        updateWindowTitle();
        
        // Update status bar
        statusBar()->showMessage(tr("Ready"));
        
        // Update view
        if (view) {
            view->retranslateUi();
        }
        
        // Save language preference
        QSettings settings;
        settings.setValue("language", language);
        
        // Force update of all widgets
        QEvent languageChangeEvent(QEvent::LanguageChange);
        QApplication::sendEvent(this, &languageChangeEvent);
        QApplication::sendEvent(view, &languageChangeEvent);
    } else {
        qWarning() << "Failed to load translation for language:" << language << "from path:" << translationPath;
    }
}

void MainWindow::onSearch()
{
    QSortFilterProxyModel* proxy = currentTabProxy();
    BuyerModel* tabModel = currentTabModel();
    if (!proxy || !tabModel) return;
    bool ok;
    QString searchText = QInputDialog::getText(this, tr("Поиск"),
                                             tr("Введите текст поиска:"), QLineEdit::Normal,
        QString(), &ok);
    if (ok && !searchText.isEmpty()) {
        int column = view->getCurrentSortColumn();
        if (column == -1) {
            QStringList columns;
            for (int i = 0; i < tabModel->columnCount(); ++i) {
                columns << tabModel->headerData(i, Qt::Horizontal).toString();
            }
            column = QInputDialog::getItem(this, tr("Выберите столбец"),
                                         tr("Поиск в столбце:"), columns, 0, false, &ok).toInt();
        }
        if (ok) {
            proxy->setFilterKeyColumn(column);
            proxy->setFilterFixedString(searchText);
        }
    }
}

void MainWindow::onPrint()
{
    BuyerModel* tabModel = currentTabModel();
    if (!tabModel) return;
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QTextDocument document;
        QString html = "<table border='1'>";
        html += "<tr>";
        for (int i = 0; i < tabModel->columnCount(); ++i) {
            html += "<th>" + tabModel->headerData(i, Qt::Horizontal).toString() + "</th>";
        }
        html += "</tr>";
        for (int row = 0; row < tabModel->rowCount(); ++row) {
            html += "<tr>";
            for (int col = 0; col < tabModel->columnCount(); ++col) {
                QModelIndex index = tabModel->index(row, col);
                html += "<td>" + index.data().toString() + "</td>";
            }
            html += "</tr>";
        }
        html += "</table>";
        document.setHtml(html);
        document.drawContents(&painter);
    }
}

void MainWindow::onShowChart()
{
    BuyerModel* tabModel = currentTabModel();
    if (!tabModel) return;

    // Create chart
    QChart *chart = new QChart();
    chart->setTitle(tr("Статистика по типам недвижимости"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create bar sets for count and average price
    QBarSet *countSet = new QBarSet(tr("Количество покупок"));
    QBarSet *priceSet = new QBarSet(tr("Средняя цена (млн руб.)"));

    // Collect data
    QMap<QString, int> typeCount;
    QMap<QString, double> typeTotalPrice;
    QStringList categories;

    for (int i = 0; i < tabModel->rowCount(); ++i) {
        Buyer buyer = tabModel->getBuyer(i);
        QString type = buyer.propertyType;
        double price = buyer.propertyPrice.toDouble();

        typeCount[type]++;
        typeTotalPrice[type] += price;
    }

    // Calculate averages and prepare data
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        QString type = it.key();
        int count = it.value();
        double avgPrice = typeTotalPrice[type] / count / 1000000.0; // Convert to millions

        categories << type;
        *countSet << count;
        *priceSet << avgPrice;
    }

    // Create series
    QBarSeries *series = new QBarSeries();
    series->append(countSet);
    series->append(priceSet);

    // Add series to chart
    chart->addSeries(series);

    // Set up axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, qMax(countSet->at(countSet->count() - 1), priceSet->at(priceSet->count() - 1)) * 1.1);
    axisY->setTitleText(tr("Значение"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Add legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create and show chart window
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *chartWindow = new QMainWindow(this);
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(1000, 600);
    chartWindow->setWindowTitle(tr("Статистика по типам недвижимости"));
    chartWindow->show();
}

void MainWindow::updateWindowTitle()
{
    QString title = tr("База данных покупателей недвижимости");
    if (!currentFile.isEmpty()) {
        title += " - " + QFileInfo(currentFile).fileName();
    }
    setWindowTitle(title);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::onSave()
{
    if (currentFile.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Сохранить файл"), "",
            tr("Текстовые файлы (*.txt);;Все файлы (*)"));
        if (!fileName.isEmpty()) {
            currentFile = fileName;
        } else {
            return;
        }
    }
    
    QFile file(currentFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < model->rowCount(); ++row) {
            Buyer buyer = model->getBuyer(row);
            out << buyer.firstName << "\t"
                << buyer.propertyAddress << "\t"
                << buyer.phoneNumber << "\t"
                << buyer.email << "\t"
                << buyer.contractNumber << "\n";
        }
        file.close();
        updateWindowTitle();
    }
} 